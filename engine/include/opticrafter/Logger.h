#pragma once

#include <spdlog/spdlog.h>

namespace opticrafter {

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
};

class Logger {
   public:
    static void init();
    static spdlog::logger& getCoreLogger();
    static spdlog::logger& getAppLogger();
    static void setCoreLoggerLevel(LogLevel level);
    static void setAppLoggerLevel(LogLevel level);
};

}  // namespace opticrafter

#define LOG_TRACE(...) opticrafter::Logger::getAppLogger().trace(__VA_ARGS__)
#define LOG_DEBUG(...) opticrafter::Logger::getAppLogger().debug(__VA_ARGS__)
#define LOG_INFO(...) opticrafter::Logger::getAppLogger().info(__VA_ARGS__)
#define LOG_WARN(...) opticrafter::Logger::getAppLogger().warn(__VA_ARGS__)
#define LOG_ERROR(...) opticrafter::Logger::getAppLogger().error(__VA_ARGS__)
#define LOG_CRITICAL(...) opticrafter::Logger::getAppLogger().critical(__VA_ARGS__)

#define LOG_CORE_TRACE(...) Logger::getCoreLogger().trace(__VA_ARGS__)
#define LOG_CORE_DEBUG(...) Logger::getCoreLogger().debug(__VA_ARGS__)
#define LOG_CORE_INFO(...) Logger::getCoreLogger().info(__VA_ARGS__)
#define LOG_CORE_WARN(...) Logger::getCoreLogger().warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) Logger::getCoreLogger().error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) Logger::getCoreLogger().critical(__VA_ARGS__)