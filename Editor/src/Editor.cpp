#include <Ripple.h>
#include <Ripple/EntryPoint.h>

#include "EditorLayer.h"

class EditorApplication : public Ripple::Application
{
public:
	EditorApplication(const Ripple::ApplicationProps& props)
		: Application(props)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new Ripple::EditorLayer());
	}
};

Ripple::Application* Ripple::CreateApplication()
{
	return new EditorApplication({"Editor", 1920, 1080});
}
