#pragma once

#ifdef RP_PLATFORM_WINDOWS

extern Ripple::Application* Ripple::CreateApplication();

int main(int argc, char** argv)
{
	Ripple::Log::Init();
	//RP_CORE_WARN("Initialized Log!");
	//int a = 5;
	//RP_INFO("Hello! Var={0}", a);

	auto app = Ripple::CreateApplication();
	app->Run();
	delete app;
}

#endif
