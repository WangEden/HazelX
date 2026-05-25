#pragma once

// 管理输入的类应该是全局的，是 static，一个应用程序只有一个
// 窗口则不一样，可以有多个

#include "KeyCodes.h"

namespace Ripple {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};

}
