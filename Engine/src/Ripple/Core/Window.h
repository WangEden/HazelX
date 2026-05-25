#pragma once

#include <functional>

#include "Ripple/Core/Base.h"
#include "Ripple/Core/Events/Event.h"

namespace Ripple {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool MaximizeWindow;

		WindowProps(const std::string& title = "Ripple Engine",
					unsigned int width = 1920,
					unsigned int height = 1080,
					bool max_window = false)
			: Title(title), Width(width), Height(height), MaximizeWindow(max_window)
		{
		}
	};

	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>; // 事件回调函数

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float, float> GetWindowPos() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		// 需要具体平台具体实现这个Create函数
		static Window* Create(const WindowProps& props = WindowProps());
	};

}
