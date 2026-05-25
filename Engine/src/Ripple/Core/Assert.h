#pragma once

//#define RP_DEBUG

#ifdef RP_DEBUG
#define RP_ENABLE_ASSERTS
#endif

#ifdef RP_ENABLE_ASSERTS
	#define RP_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { RP_ERROR("Assertion Failed!"); __debugbreak(); } }
	#define RP_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { RP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#define RP_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro

	#define RP_ASSERT(...) RP_ASSERT_RESOLVE(__VA_ARGS__, RP_ASSERT_MESSAGE, RP_ASSERT_NO_MESSAGE)(__VA_ARGS__)
	#define RP_CORE_ASSERT(...) RP_ASSERT_RESOLVE(__VA_ARGS__, RP_ASSERT_MESSAGE, RP_ASSERT_NO_MESSAGE)(__VA_ARGS__)
#else
	#define RP_ASSERT(x, ...)
	#define RP_CORE_ASSERT(x, ...)
#endif
