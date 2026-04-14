#include "hzpch.h"
#include "FluidSimLayer.h"

namespace Hazel {

	FluidSimLayer::FluidSimLayer()
		// 初始化相机为一个 2D 正交投影，视野大小为宽 32，高 18
		: m_Camera(-16.0f, 16.0f, -9.0f, 9.0f)
	{
	}

	FluidSimLayer::~FluidSimLayer()
	{
	}

	void FluidSimLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;

		fbSpec.Format = FramebufferFormat::RGBA8;
		fbSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		m_Framebuffer = Framebuffer::Create(fbSpec);

		ResetParameter();
		ResetParticles();
	}

	void FluidSimLayer::OnDetach()
	{
	}

	void FluidSimLayer::ResetParticles()
	{
		m_Particles.clear();

		int particlesPerRow = (int)std::sqrt(m_ParticleCount);
		int particlesPerCol = (m_ParticleCount + particlesPerRow - 1) / particlesPerRow;

		float spacing = m_ParticleRadius * 2.0f;

		float startX = -(particlesPerRow * spacing) / 2.0f;
		float startY = -(particlesPerCol * spacing) / 2.0f + 2.0f; // 稍微抬高一点，不直接贴底

		for (int y = 0; y < particlesPerCol; y++) {
			for (int x = 0; x < particlesPerRow; x++) {
				if (m_Particles.size() >= m_ParticleCount) break;

				m_Particles.push_back({
					{ startX + x * spacing, startY + y * spacing },
					{ 0.0f, 0.0f },
					0.0f,
					0.0f
				});
			}
		}

		m_ParticleCount = (int)m_Particles.size();
	}

	void FluidSimLayer::ResetAll()
	{
		ResetParameter();
		ResetParticles();
	}

	void FluidSimLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			float aspectRatio = m_ViewportSize.x / m_ViewportSize.y;
			float orthoSize = 10.0f; // 相机视野高度的一半
			m_Camera.SetProjection(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize);
		}

		float dt = std::min(ts.GetSeconds(), 0.016f);
		float invH = 1.0f / m_SmoothingRadius;

		BuildSpatialHash();

		// 1. 计算所有粒子的密度和压力
		for (int i = 0; i < m_Particles.size(); i++)
		{
			auto& p = m_Particles[i];
			float density = 0.0f;

			int gx = (int)std::floor(p.Position.x * invH);
			int gy = (int)std::floor(p.Position.y * invH);

			for (int x = -m_HashGridStep; x <= m_HashGridStep; x++) {
				for (int y = -m_HashGridStep; y <= m_HashGridStep; y++) {
					uint32_t hash = CalculateHash(gx + x, gy + y);
					UINT32 startIdx = m_CellStart[hash];
					if (startIdx == 0xFFFFFFFF) continue; // 无效索引

					for (uint32_t k = startIdx; k < m_SortedEntries.size() && m_SortedEntries[k].Hash == hash; k++) {
						int j = m_SortedEntries[k].Index;
						float dist = glm::distance(p.Position, m_Particles[j].Position);
						if (dist < m_SmoothingRadius) {
							density += Physics2D::SmoothingKernel(m_SmoothingRadius, dist);
						}
					}
				}
			}
			p.Density = density;
			p.Property = std::max(0.0f, (p.Density - m_TargetDensity)) * m_PressureMultiplier; // P = k * (rho - rho0)
		}

		// 2. 计算受力并更新速度 (Pressure + Viscosity + Gravity)
		for (int i = 0; i < m_Particles.size(); i++)
		{
			auto& p = m_Particles[i];
			glm::vec2 pressureForce(0.0f);
			glm::vec2 viscosityForce(0.0f);

			int gx = (int)std::floor(p.Position.x * invH);
			int gy = (int)std::floor(p.Position.y * invH);

			for (int x = -m_HashGridStep; x <= m_HashGridStep; ++x) {
				for (int y = -m_HashGridStep; y <= m_HashGridStep; ++y) {
					uint32_t hash = CalculateHash(gx + x, gy + y);
					uint32_t startIdx = m_CellStart[hash];
					if (startIdx == 0xFFFFFFFF) continue; // 无效索引

					for (uint32_t k = startIdx; k < m_SortedEntries.size() && m_SortedEntries[k].Hash == hash; k++) {
						int j = m_SortedEntries[k].Index;
						if (i == j) continue;

						float dist = glm::distance(p.Position, m_Particles[j].Position);
						if (dist >= m_SmoothingRadius || dist < 1e-5f) continue;

						glm::vec2 dir = (p.Position - m_Particles[j].Position) / dist;
						float slope = Physics2D::SmoothingKernelDerivative(m_SmoothingRadius, dist);

						float sharedPressure = (p.Property + m_Particles[j].Property) / 2.0f;
						pressureForce += dir * sharedPressure * slope / m_Particles[j].Density;

						viscosityForce += (m_Particles[j].Velocity - p.Velocity) * Physics2D::SmoothingKernel(m_SmoothingRadius, dist);
					}
				}
			}

			glm::vec2 acceleration = (pressureForce / m_Particles[i].Density) + (viscosityForce * m_ViscosityStrength);
			acceleration.y += m_Gravity;
			p.Velocity += acceleration * dt;
		}

		// 3. 更新位置与边界处理
		for (auto& p : m_Particles) {
			p.Position += p.Velocity * dt;
			Physics2D::ResolveBoundaryCollision(
				p.Position, p.Velocity, m_ParticleRadius,
				{ -m_BoxWidth / 2.0f, -m_BoxHeight / 2.0f },
				{ m_BoxWidth / 2.0f, m_BoxHeight / 2.0f }, m_CollisionDamping);

			if (m_Obstacle.Enabled) {
				Physics2D::ResolveCircleCollision(
					p.Position, p.Velocity, m_ParticleRadius,
					m_Obstacle.Center, m_Obstacle.Radius, m_CollisionDamping
				);
			}
		}

		// --- 渲染部分 ---
		m_Framebuffer->Bind();
		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();
		Renderer2D::BeginScene(m_Camera.GetViewProjectionMatrix());
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { m_BoxWidth, m_BoxHeight }, { 0.3f, 0.3f, 0.3f, 0.5f });

		if (m_Obstacle.Enabled) {
			Renderer2D::DrawCircle(m_Obstacle.Center, m_Obstacle.Radius, { 0.8f, 0.2f, 0.2f, 1.0f });
		}

		for (const auto& p : m_Particles) {
			float colorVal = std::clamp(p.Density / m_TargetDensity, 0.5f, 1.5f);
			//Renderer2D::DrawCircle(p.Position, m_ParticleRadius, { 0.2f * (2 - colorVal), 0.6f * (2 - colorVal), 1.0f, 1.0f }); // 越蓝密度越高
			Renderer2D::DrawCircle(p.Position, m_ParticleRadius, { 0.2f * colorVal, 0.6f * colorVal, 1.0f, 1.0f }); // 越白密度越高
		}

		Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void FluidSimLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// ==================== 1. 设置面板 (Inspector) ====================
		ImGui::Begin("Fluid Settings");

		ImGui::Text("Simulation Parameters");
		//ImGui::DragFloat("Gravity", &m_Gravity, 0.1f, -20.0f, 20.0f);
		ImGui::DragFloat("Particle Radius", &m_ParticleRadius, 0.01f, 0.01f, 2.5f);
		ImGui::DragFloat("Collision Damping", &m_CollisionDamping, 0.01f, 0.01f, 1.0f);

		ImGui::Separator();
		ImGui::Text("SPH Parameters");
		ImGui::DragFloat("SmoothingRadius", &m_SmoothingRadius, 0.01f, 0.02f, 5.0f);
		ImGui::DragFloat("TargetDensity", &m_TargetDensity, 0.01f, 0.01f, 20.0f);
		ImGui::DragFloat("PressureMultiplier", &m_PressureMultiplier, 0.01f, 1.0f, 100.0f);
		ImGui::DragFloat("ViscosityStrength", &m_ViscosityStrength, 0.01f, 0.01f, 10.0f);

		ImGui::Separator();
		ImGui::Text("Environment");
		ImGui::DragFloat("Box Width", &m_BoxWidth, 0.1f, 0.1f, 50.0f);
		ImGui::DragFloat("Box Height", &m_BoxHeight, 0.1f, 0.1f, 50.0f);

		ImGui::Separator();
		ImGui::Text("Obstacle");
		ImGui::Checkbox("Enable Obstacle", &m_Obstacle.Enabled);
		ImGui::DragFloat2("Pos", glm::value_ptr(m_Obstacle.Center), 0.1f);
		ImGui::DragFloat("Radius", &m_Obstacle.Radius, 0.01f, 0.01f, 5.0f);

		ImGui::Separator();
		if (ImGui::Button("Reset Simulation")) {
			ResetParticles();
		}
		if (ImGui::Button("Reset All")) {
			ResetAll();
		}
		ImGui::Text("Particle Count: %d", m_ParticleCount);
		ImGui::End();

		// ==================== 2. 渲染视口 (Viewport) ====================
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End(); // End DockSpace
	}

	void FluidSimLayer::OnEvent(Event& event)
	{
	}

}
