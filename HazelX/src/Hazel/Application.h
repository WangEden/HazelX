#pragma once

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	// To be defined in CLIEN 请在客户端定义这些
	Application * CreateApplication();
}


