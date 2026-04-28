#include "opticrafter/Logger.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cassert>

namespace opticrafter {

std::shared_ptr<spdlog::logger> g_core_logger = nullptr;
std::shared_ptr<spdlog::logger> g_app_logger = nullptr;

static spdlog::level::level_enum toSpdlogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:
            return spdlog::level::trace;
        case LogLevel::DEBUG:
            return spdlog::level::debug;
        case LogLevel::INFO:
            return spdlog::level::info;
        case LogLevel::WARN:
            return spdlog::level::warn;
        case LogLevel::ERROR:
            return spdlog::level::err;
        case LogLevel::CRITICAL:
            return spdlog::level::critical;
    }
}

void Logger::init() {
    if (g_core_logger != nullptr || g_app_logger != nullptr) {
        LOG_CORE_WARN("Logger already initialized.");
        return;
    }

    g_core_logger = spdlog::stdout_color_mt("CORE");
    g_app_logger = spdlog::stdout_color_mt("APP");

    g_core_logger->set_pattern("%^[%n] (%l)\t%v%$");
    g_app_logger->set_pattern("%^[%n] (%l)\t%v%$");

#ifndef NDEBUG
    g_core_logger->set_level(spdlog::level::trace);
#else
    g_core_logger->set_level(spdlog::level::info);
#endif

    LOG_CORE_DEBUG("Logger init successful!");
}

spdlog::logger& Logger::getCoreLogger() {
    assert(g_core_logger);
    return *g_core_logger;
}

spdlog::logger& Logger::getAppLogger() {
    assert(g_app_logger);
    return *g_app_logger;
}

void Logger::setCoreLoggerLevel(LogLevel level) { g_core_logger->set_level(toSpdlogLevel(level)); }

void Logger::setAppLoggerLevel(LogLevel level) { g_app_logger->set_level(toSpdlogLevel(level)); }

}  // namespace opticrafter