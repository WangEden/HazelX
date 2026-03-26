#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel {

	// 不需要导出到dll
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

		//virtual float GetMouseScrollYImpl() override;
	};
}
