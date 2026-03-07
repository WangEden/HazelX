#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

	// 当前事件系统是阻塞式的，意味着，当事件发生时需要立即获取分发并处理
	// 未来将通过在事件总线中添加事件缓冲的方式改进

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory // 位字段，这样事件就能属于多个类别 0001 | 0010 → 0011
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
		//friend class EventDispatcher;
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0; // const = 0 表示纯虚函数，即必须被实现
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	//protected:
		//bool m_Handled = false; // 规定事件是否想被继续传播，这样下面的层就不会再重复处理事件
		// 事件会在层之间传播
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>; // 创建一个别名，指定一种函数类型
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{ 
		}

		template<typename T>
		bool Dispatch(EventFn<T> func) // 接收的一个参数为bool类型的函数
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event); 
				// *(T*)&m_Event 将基类指针转换成派生类指针再解引用为T&，并作为参数调用
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

#include "spdlog/fmt/fmt.h"

// 使用模板匹配 Hazel::Event 及其所有派生类
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<Hazel::Event, T>::value, char>>
	: fmt::formatter<std::string> {

	template <typename FormatContext>
	auto format(const Hazel::Event& e, FormatContext& ctx) const {
		return fmt::formatter<std::string>::format(e.ToString(), ctx);
	}
};