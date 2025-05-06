#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

class log
{
public:
    static void Init();

    // Engine logger access
    inline static std::shared_ptr<spdlog::logger>& getEngineLogger() {
        return engineLogger;
    }

    // Lua logger access
    inline static std::shared_ptr<spdlog::logger>& getLuaLogger() {
        return luaLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> engineLogger;
    static std::shared_ptr<spdlog::logger> luaLogger;
};

// Engine log macros
#define LOG_TRACE(...)    log::getEngineLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     log::getEngineLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     log::getEngineLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    log::getEngineLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)    log::getEngineLogger()->critical(__VA_ARGS__)

// Lua log macros
#define LUA_TRACE(...)    log::getLuaLogger()->trace(__VA_ARGS__)
#define LUA_INFO(...)     log::getLuaLogger()->info(__VA_ARGS__)
#define LUA_WARN(...)     log::getLuaLogger()->warn(__VA_ARGS__)
#define LUA_ERROR(...)    log::getLuaLogger()->error(__VA_ARGS__)
#define LUA_FATAL(...)    log::getLuaLogger()->critical(__VA_ARGS__)