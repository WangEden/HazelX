#include <Hazel.h>
#include <Hazel/EntryPoint.h>

#include "EditorLayer.h"
#include "FluidSimLayer.h"

class HazelnutApplication : public Hazel::Application
{
public:
	HazelnutApplication(const Hazel::ApplicationProps& props)
		: Application(props)
	{
	}

	virtual void OnInit() override
	{
		//PushLayer(new Hazel::EditorLayer());
		PushLayer(new Hazel::FluidSimLayer());
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new HazelnutApplication({"Hazelnut", 1600, 900});
}
