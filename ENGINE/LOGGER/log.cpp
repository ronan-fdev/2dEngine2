#include "log.h"

std::shared_ptr<spdlog::logger> log::logger;

void log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	logger = spdlog::stderr_color_mt("ENGINE");
	logger->set_level(spdlog::level::trace);
}
