#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		// 将事件回调函数绑定到窗口事件系统中，这样当窗口事件发生时就会调用Application的OnEvent方法
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/* OpenGL 渲染管线
			1.顶点数组、2.顶点缓冲、3.索引缓冲
			4.着色器（可不写，GPU可能有默认着色器）
		*/
		glGenVertexArrays(1, &m_VertexArray); // 生成一个顶点数组对象
		glBindVertexArray(m_VertexArray); // 绑定顶点数组对象

		glGenBuffers(1, &m_VertexBuffer); // 生成一个顶点缓冲对象
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer); // 绑定顶点缓冲区

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f, // 左下
			 0.5f, -0.5f, 0.0f, // 右下
			 0.0f,  0.5f, 0.0f  // 上
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 将顶点数据传输到GPU, GL_STATIC_DRAW表示数据不会频繁修改
		glEnableVertexAttribArray(0); // 启用顶点属性数组，参数0表示第一个属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); // 定义顶点属性布局

		glGenBuffers(1, &m_IndexBuffer); // 生成一个索引缓冲区(OpenGL中叫做元素缓冲区)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); // 绑定索引缓冲区1

		unsigned int indices[3] = { 0, 1, 2 }; // 定义索引数据，坐下开始逆时针绘制三角形
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 将索引数据传输到GPU

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position; // 传递给片段着色器的变量
		
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr); // 绘制三角形，参数1表示绘制模式，参数2表示索引数量，参数3表示索引数据类型，参数4表示索引数据在缓冲区中的偏移量
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(); // 提交要渲染的层

			m_ImGuiLayer->Begin();
			for(Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


}
