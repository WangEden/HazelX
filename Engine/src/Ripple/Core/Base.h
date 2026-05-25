#pragma once

#include <memory>
#include "Ref.h"

namespace Ripple {

	void InitializeCore();
	void ShutdownCore();

}

#ifndef RP_PLATFORM_WINDOWS
	#error Ripple only supports Windows!
#endif

#define BIT(x) (1 << x)

#define RP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include "Assert.h"

namespace Ripple {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	using byte = uint8_t;

}
