#pragma once

#include "Hazel/Core/Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Hazel/Core/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void RenderImGui();

		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		static Application* s_Instance;
	};

	// To be defined in CLIEN 请在客户端定义这些
	Application * CreateApplication();
}
