#pragma once

#include "Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ripple {

	class Log
	{
	public:
        static void Init();

		// 下面这里返回的是一个智能指针的引用，不会增加引用计数
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; } 
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return  s_ClientLogger; }
    private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
// 核心 Logger 宏
#define  RP_CORE_TRACE(...) ::Ripple::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define  RP_CORE_INFO(...)  ::Ripple::Log::GetCoreLogger()->info(__VA_ARGS__)
#define  RP_CORE_WARN(...)  ::Ripple::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define  RP_CORE_ERROR(...) ::Ripple::Log::GetCoreLogger()->error(__VA_ARGS__)
#define  RP_CORE_FATAL(...) ::Ripple::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// 客户端 Logger 宏
#define  RP_TRACE(...) ::Ripple::Log::GetClientLogger()->trace(__VA_ARGS__)
#define  RP_INFO(...)  ::Ripple::Log::GetClientLogger()->info(__VA_ARGS__)
#define  RP_WARN(...)  ::Ripple::Log::GetClientLogger()->warn(__VA_ARGS__)
#define  RP_ERROR(...) ::Ripple::Log::GetClientLogger()->error(__VA_ARGS__)
#define  RP_FATAL(...) ::Ripple::Log::GetClientLogger()->fatal(__VA_ARGS__)
