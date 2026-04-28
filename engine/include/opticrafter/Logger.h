#pragma once

#include <spdlog/spdlog.h>

namespace opticrafter {
class Logger {
   public:
    static void init();
    static spdlog::logger& getCoreLogger();
    static spdlog::logger& getAppLogger();
};

}  // namespace opticrafter

#define LOG_TRACE(...) Logger::getAppLogger().trace(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::getAppLogger().debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::getAppLogger().info(__VA_ARGS__)
#define LOG_WARN(...) Logger::getAppLogger().warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getAppLogger().error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::getAppLogger().critical(__VA_ARGS__)

#define LOG_CORE_TRACE(...) Logger::getCoreLogger().trace(__VA_ARGS__)
#define LOG_CORE_DEBUG(...) Logger::getCoreLogger().debug(__VA_ARGS__)
#define LOG_CORE_INFO(...) Logger::getCoreLogger().info(__VA_ARGS__)
#define LOG_CORE_WARN(...) Logger::getCoreLogger().warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) Logger::getCoreLogger().error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) Logger::getCoreLogger().critical(__VA_ARGS__)