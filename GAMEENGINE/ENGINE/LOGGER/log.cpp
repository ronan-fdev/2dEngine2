#include "log.h"

std::shared_ptr<spdlog::logger> log::engineLogger;
std::shared_ptr<spdlog::logger> log::luaLogger;

void log::Init()
{
    // Set common logging pattern
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // Initialize engine logger
    engineLogger = spdlog::stderr_color_mt("ENGINE");
    engineLogger->set_level(spdlog::level::trace);

    // Initialize Lua logger
    luaLogger = spdlog::stderr_color_mt("LUA");
    luaLogger->set_level(spdlog::level::trace);
}