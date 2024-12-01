#include "Log.hpp"

namespace Engine {

std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
std::shared_ptr<spdlog::logger> Log::m_ClientLogger;
bool Log::m_Initialized = false;

void Log::Init() {
    if (m_Initialized) {
        CORE_WARN("Logger already initialized.");
        return;
    }

    spdlog::set_pattern("%^[%T] %n: %v%$");  // [HH:MM:SS] loggerName: log

    m_CoreLogger = spdlog::stdout_color_mt("ENGINE");
    m_ClientLogger = spdlog::stdout_color_mt("APP");

#ifdef ENG_DEBUG
    m_CoreLogger->set_level(spdlog::level::trace);
    m_ClientLogger->set_level(spdlog::level::trace);
#endif

    CORE_INFO("Engine logger initialized");
    APP_INFO("Application logger initialized");

    m_Initialized = true;
}

}  // namespace Engine
