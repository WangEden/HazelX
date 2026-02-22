#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIEN 请在客户端定义这些
	Application * CreateApplication();
}
