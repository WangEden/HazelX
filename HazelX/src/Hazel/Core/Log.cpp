#include "hzpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h" // 有这个才能设置级别

namespace Hazel {
	
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // 设置输出文本的模版格式
		s_CoreLogger = spdlog::stdout_color_mt("Hazel"); // mt表示多线程
		s_CoreLogger->set_level(spdlog::level::trace); // 级别设置为所有消息都打印

		s_ClientLogger = spdlog::stderr_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}