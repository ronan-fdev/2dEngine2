#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <sstream>

class log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }
private:
	static std::shared_ptr<spdlog::logger> logger;
};

//ENGINE log macros:
#define LOG_TRACE(...) log::getLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...) log::getLogger()->info(__VA_ARGS__);
#define LOG_WARN(...) log::getLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...) log::getLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...) log::getLogger()->fatal(__VA_ARGS__);