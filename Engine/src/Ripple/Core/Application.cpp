#include "hzpch.h"
#include "Application.h"

#include "Ripple/Renderer/Renderer.h"
#include "Ripple/Renderer/Framebuffer.h"
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "Ripple/Script/ScriptEngine.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

// 暂时
#include <glad/glad.h>

namespace Ripple {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationProps& props)
	{
		RP_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(props.Name, props.WindowWidth, props.WindowHeight)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); // 将事件回调函数绑定到窗口事件系统中，这样当窗口事件发生时就会调用Application的OnEvent方法
		m_Window->SetVSync(true);

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		ScriptEngine::Init("assets/scripts/ExampleApp.dll");

		Renderer::Init();
		Renderer::WaitAndRender();
	}

	Application::~Application()
	{
		ScriptEngine::Shutdown();

		for (Layer* layer : m_LayerStack) {
			layer->OnDetach();
			delete layer;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();

		ImGui::Begin("Renderer");
		auto& caps = RendererAPI::GetCapabilities();
		ImGui::Text("Vendor: %s", caps.Vendor.c_str());
		ImGui::Text("Renderer: %s", caps.Renderer.c_str());
		ImGui::Text("Version: %s", caps.Version.c_str());
		ImGui::Text("Frame Time: %.2fms\n", m_TimeStep.GetMilliseconds());
		ImGui::End();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep); // 提交要渲染的层

				// Render ImGui on render thread
				Application* app = this;
				Renderer::Submit([app]() { app->RenderImGui(); });

				Renderer::WaitAndRender();
				// 渲染解耦，此处只负责渲染，提交渲染命令由RP_RENDER宏完成，
				// RP_RENDER宏会调用Renderer::Submit()将渲染命令提交到Renderer的命令队列中，
				// 然后Renderer::WaitAndRender()会依次执行队列中的渲染命令；
			}
			m_Window->OnUpdate();

			float time = GetTime();
			m_TimeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;
		}
		OnShutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::Submit([=]() { glViewport(0, 0, width, height); });
		auto& fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto& fb : fbs)
			fb->Resize(width, height);
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	std::string Application::OpenFile(const char* filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string Application::SaveFile(const char* filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}

	const char* Application::GetConfigurationName()
	{
#if defined(RP_DEBUG)
		return "Debug";
#elif defined(RP_RELEASE)
		return "Release";
#elif defined(RP_DIST)
		return "Dist";
#else
#error Undefined configuration?
#endif
	}

	const char* Application::GetPlatformName()
	{
#if defined(RP_PLATFORM_WINDOWS)
		return "Windows x64";
#else
#error Undefined platform?
#endif
	}
}
