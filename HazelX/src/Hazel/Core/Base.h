#pragma once

#include <memory>

namespace Hazel {

	void InitializeCore();
	void ShutdownCore();

}

#ifndef HZ_PLATFORM_WINDOWS
	#error Hazel only supports Windows!
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include "Assert.h"

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	using byte = unsigned char;

}
