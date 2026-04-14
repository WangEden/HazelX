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

		ResetParticles();
	}

	void FluidSimLayer::OnDetach()
	{
	}

	void FluidSimLayer::ResetParticles()
	{
		m_Particles.clear();

		// 重新根据设定的 m_ParticleCount 调整布局
		int particlesPerRow = (int)std::sqrt(m_ParticleCount);
		int particlesPerCol = (m_ParticleCount + particlesPerRow - 1) / particlesPerRow;

		// 关键点：初始间距不宜过大也不宜过小
		// 建议略小于 m_SmoothingRadius，确保初始就有一定的密度贡献
		float spacing = m_ParticleRadius * 2.0f;

		float startX = -(particlesPerRow * spacing) / 2.0f;
		float startY = -(particlesPerCol * spacing) / 2.0f + 2.0f; // 稍微抬高一点，不直接贴底

		for (int y = 0; y < particlesPerCol; y++) {
			for (int x = 0; x < particlesPerRow; x++) {
				if (m_Particles.size() >= m_ParticleCount) break;

				// 初始化位置、速度为0、密度为0、压力(Property)为0
				m_Particles.push_back({
					{ startX + x * spacing, startY + y * spacing },
					{ 0.0f, 0.0f },
					0.0f,
					0.0f
					});
			}
		}

		// 更新实际生成的数量
		m_ParticleCount = (int)m_Particles.size();
	}

	void FluidSimLayer::ResetParameter()
	{
		// 基础物理常数
		m_ParticleCount = PARTICLE_COUNT;
		m_Gravity = -9.81f;
		m_ParticleRadius = 0.08f;
		m_CollisionDamping = 0.5f;

		// SPH 特有参数
		m_SmoothingRadius = 0.25f;
		m_TargetDensity = 1.0f;
		m_PressureMultiplier = 200.0f;
		m_ViscosityStrength = 0.05f;

		// 环境尺寸
		m_BoxWidth = 20.0f;
		m_BoxHeight = 15.0f;

		// 核心：必须重新计算与半径相关的缩放因子
		UpdateScalingFactors();
		ResetParticles();
	}

	void FluidSimLayer::OnUpdate(Timestep ts)
	{
		// 处理视口大小改变
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			// 调整正交相机的宽高比边界，防止画面拉伸
			float aspectRatio = m_ViewportSize.x / m_ViewportSize.y;
			float orthoSize = 10.0f; // 相机视野高度的一半
			m_Camera.SetProjection(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize);
		}

		// 静态圆测试
		//Renderer2D::DrawCircle({ 0.0f, 0.0f }, 2.0f, { 1.0f, 0.0f, 0.0f, 1.0f });

		float dt = std::min(ts.GetSeconds(), 0.016f); // 锁定步长防止物理崩溃

		// 1. 计算所有粒子的密度和压力
		for (auto& p : m_Particles) {
			p.Density = Physics2D::CalculateDensity(p, m_Particles, m_SmoothingRadius, m_Poly6ScalingFactor);
			// 简单的状态方程：P = k * (rho - rho0)
			p.Property = (p.Density - m_TargetDensity) * m_PressureMultiplier;
		}

		// 2. 计算受力并更新速度 (Pressure + Viscosity + Gravity)
		for (int i = 0; i < m_Particles.size(); i++) {
			glm::vec2 pressureForce(0.0f);
			glm::vec2 viscosityForce(0.0f);

			for (int j = 0; j < m_Particles.size(); j++) {
				if (i == j) continue;

				float dist = glm::distance(m_Particles[i].Position, m_Particles[j].Position);
				if (dist > m_SmoothingRadius || dist < 0.0001f) continue;

				glm::vec2 dir = (m_Particles[j].Position - m_Particles[i].Position) / dist;
				float slope = Physics2D::SmoothingKernelDerivative(m_SmoothingRadius, dist);

				// 压力项：使用对称公式防止单向加速
				float sharedPressure = (m_Particles[i].Property + m_Particles[j].Property) / 2.0f;
				pressureForce += dir * sharedPressure * slope / m_Particles[j].Density;

				// 粘性项：拉近速度差
				viscosityForce += (m_Particles[j].Velocity - m_Particles[i].Velocity) * Physics2D::SmoothingKernel(m_SmoothingRadius, dist, m_Poly6ScalingFactor);
			}

			glm::vec2 acceleration = (pressureForce / m_Particles[i].Density) + (viscosityForce * m_ViscosityStrength);
			acceleration.y += m_Gravity;

			m_Particles[i].Velocity += acceleration * dt;
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
			// 根据密度改变颜色，方便观察
			float colorVal = std::clamp(p.Density / m_TargetDensity, 0.5f, 1.5f);
			Renderer2D::DrawCircle(p.Position, m_ParticleRadius, { 0.2f * colorVal, 0.6f * colorVal, 1.0f, 1.0f });
		}

		Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void FluidSimLayer::OnImGuiRender()
	{
		// --- 开启 DockSpace (允许窗口停靠) ---
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
		ImGui::DragFloat("Gravity", &m_Gravity, 0.1f, -20.0f, 20.0f);
		ImGui::DragFloat("Particle Radius", &m_ParticleRadius, 0.01f, 0.05f, 1.0f);
		ImGui::DragFloat("Collision Damping", &m_CollisionDamping, 0.01f, 0.0f, 1.0f);

		ImGui::Separator();
		ImGui::Text("SPH Parameters");
		ImGui::DragFloat("SmoothingRadius", &m_SmoothingRadius, 0.01f, 0.1f, 0.5f);
		ImGui::DragFloat("TargetDensity", &m_TargetDensity, 0.01f, 0.1f, 2.0f);
		ImGui::DragFloat("PressureMultiplier", &m_PressureMultiplier, 10.0f, 100.0f, 500.0f);
		ImGui::DragFloat("ViscosityStrength", &m_ViscosityStrength, 0.01f, 0.01f, 0.2f);

		ImGui::Separator();
		ImGui::Text("Environment");
		ImGui::DragFloat("Box Width", &m_BoxWidth, 0.1f, 5.0f, 50.0f);
		ImGui::DragFloat("Box Height", &m_BoxHeight, 0.1f, 5.0f, 50.0f);

		ImGui::Separator();
		ImGui::Text("Obstacle");
		ImGui::Checkbox("Enable Obstacle", &m_Obstacle.Enabled);
		ImGui::DragFloat2("Pos", glm::value_ptr(m_Obstacle.Center), 0.1f);
		ImGui::DragFloat("Radius", &m_Obstacle.Radius, 0.1f, 0.5f, 5.0f);

		ImGui::Separator();
		if (ImGui::Button("Reset Simulation")) {
			ResetParticles();
		}
		if (ImGui::Button("Reset Parameters")) {
			ResetParameter();
		}
		ImGui::Text("Particle Count: %d", m_ParticleCount);
		ImGui::End();

		// ==================== 2. 渲染视口 (Viewport) ====================
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// 将 Framebuffer 的纹理绘制到 ImGui 窗口中
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		// 注意 UV 坐标，OpenGL 纹理原点在左下角，ImGui 在左上角，需要翻转 Y 轴
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End(); // End DockSpace
	}

	void FluidSimLayer::OnEvent(Event& event)
	{
		// 如果你想加鼠标平移缩放 2D 相机，可以在这里拦截事件并修改 m_Camera 的投影矩阵
	}

}
