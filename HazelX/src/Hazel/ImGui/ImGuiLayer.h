#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Core/Events/ApplicationEvent.h"
#include "Hazel/Core/Events/KeyEvent.h"
#include "Hazel/Core/Events/MouseEvent.h"

struct ImGuiIO;

namespace Hazel {

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
