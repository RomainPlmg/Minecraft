#include "opticrafter/Logger.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cassert>

namespace opticrafter {

std::shared_ptr<spdlog::logger> g_core_logger = nullptr;
std::shared_ptr<spdlog::logger> g_app_logger = nullptr;

void Logger::init() {
    if (g_core_logger != nullptr || g_app_logger != nullptr) {
        LOG_WARN("Logger already initialized.");
        return;
    }

    g_core_logger = spdlog::stdout_color_mt("CORE");
    g_app_logger = spdlog::stdout_color_mt("APP");

    g_core_logger->set_pattern("%^[%n] (%l) %v%$");
    g_app_logger->set_pattern("%^[%n] (%l) %v%$");

#ifndef NDEBUG
    g_core_logger->set_level(spdlog::level::trace);
#else
    g_core_logger->set_level(spdlog::level::info);
#endif

    LOG_CORE_INFO("Logger initialized.");
}

spdlog::logger& Logger::getCoreLogger() {
    assert(g_core_logger);
    return *g_core_logger;
}

spdlog::logger& Logger::getAppLogger() {
    assert(g_core_logger);
    return *g_core_logger;
}

}  // namespace opticrafter