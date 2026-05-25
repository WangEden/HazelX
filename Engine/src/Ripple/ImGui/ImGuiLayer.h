#pragma once

#include "Ripple/Core/Layer.h"

#include "Ripple/Core/Events/ApplicationEvent.h"
#include "Ripple/Core/Events/KeyEvent.h"
#include "Ripple/Core/Events/MouseEvent.h"

struct ImGuiIO;

namespace Ripple {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		~ImGuiLayer();

		void Begin();
		void End();

		virtual void OnAttach() override;
		virtual void OnDetach() override; 
		virtual void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};
}
