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
		m_Positions.clear();
		m_Velocities.clear();

		// 在空中生成一个粒子方阵
		int particlesSqrt = (int)std::sqrt(m_ParticleCount);
		float spacing = m_ParticleRadius * 2.2f; // 粒子间距

		float startX = -(particlesSqrt * spacing) / 2.0f;
		float startY = 0.0f;

		for (int y = 0; y < particlesSqrt; y++) {
			for (int x = 0; x < particlesSqrt; x++) {
				m_Positions.push_back({ startX + x * spacing, startY + y * spacing });
				m_Velocities.push_back({ 0.0f, 0.0f });
			}
		}
		m_ParticleCount = m_Positions.size();
	}

	void FluidSimLayer::ResetParameter()
	{
		m_ParticleCount = 1000;
		m_Gravity = -9.8f;
		m_ParticleRadius = 0.16f;
		m_CollisionDamping = 0.7f;

		m_BoxWidth = 20.0f;
		m_BoxHeight = 15.0f;

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

		// 绑定 Framebuffer，准备渲染
		m_Framebuffer->Bind();

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();
		Renderer2D::BeginScene(m_Camera.GetViewProjectionMatrix());

		// --- 3. 渲染测试 ---
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { m_BoxWidth, m_BoxHeight }, { 0.3f, 0.3f, 0.3f, 0.5f });

		// 静态圆测试
		//Renderer2D::DrawCircle({ 0.0f, 0.0f }, 2.0f, { 1.0f, 0.0f, 0.0f, 1.0f });

		float dt = ts.GetSeconds();
		if (dt > 0.033f) dt = 0.033f;

		// 批量渲染粒子
		for (int i = 0; i < m_ParticleCount; i++)
		{
			m_Velocities[i].y += m_Gravity * dt;
			m_Positions[i] += m_Velocities[i] * dt;

			// 物理：边界碰撞
			Physics2D::ResolveBoundaryCollision(
				m_Positions[i], m_Velocities[i], m_ParticleRadius,
				{ -m_BoxWidth / 2.0f, -m_BoxHeight / 2.0f },
				{ m_BoxWidth / 2.0f, m_BoxHeight / 2.0f }, m_CollisionDamping);

			// 渲染：画水粒子
			Renderer2D::DrawCircle(m_Positions[i], m_ParticleRadius, { 0.2f, 0.6f, 1.0f, 1.0f });
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
		ImGui::DragFloat("Collision Damping", &m_CollisionDamping, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Particle Radius", &m_ParticleRadius, 0.01f, 0.05f, 1.0f);

		ImGui::Separator();
		ImGui::Text("Environment");
		ImGui::DragFloat("Box Width", &m_BoxWidth, 0.1f, 5.0f, 50.0f);
		ImGui::DragFloat("Box Height", &m_BoxHeight, 0.1f, 5.0f, 50.0f);

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
