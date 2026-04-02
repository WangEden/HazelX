#pragma once

#include <memory>

namespace Hazel {

	void InitializeCore();
	void ShutdownCore();

}

// 定义一个宏决定在构建的时候是dllimport还是dllexport
#ifdef HZ_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else
			#define HAZEL_API __declspec(dllimport)
		#endif
	#else
		#define HAZEL_API
	#endif
#else
	#error Hazel only supports Windows!
#endif

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { HZ_ERROR("Assertion Failed!"); __debugbreak(); } }
	#define HZ_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#define HZ_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro

	#define HZ_ASSERT(...) HZ_ASSERT_RESOLVE(__VA_ARGS__, HZ_ASSERT_MESSAGE, HZ_ASSERT_NO_MESSAGE)(__VA_ARGS__)
	#define HZ_CORE_ASSERT(...) HZ_ASSERT_RESOLVE(__VA_ARGS__, HZ_ASSERT_MESSAGE, HZ_ASSERT_NO_MESSAGE)(__VA_ARGS__)
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	using byte = unsigned char;

}
