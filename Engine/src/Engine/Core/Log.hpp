#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Core.hpp"

namespace Engine {

class ENGINE_API Log {
   public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }
    inline static const bool IsInitialized() { return m_Initialized; }

   private:
    static std::shared_ptr<spdlog::logger> m_CoreLogger;
    static std::shared_ptr<spdlog::logger> m_ClientLogger;
    static bool m_Initialized;
};

}  // namespace Engine

// clang-format off
#ifdef ENG_DEBUG
    // Core log macros
    #define CORE_TRACE(...) ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define CORE_INFO(...)  ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define CORE_WARN(...)  ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define CORE_ERROR(...) ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define CORE_FATAL(...) ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__); \
                            DEBUGBREAK

    #define CORE_ASSERT(x, msg) if (!x) { \
                                CORE_FATAL("Assertion failed: {0}", msg); \
                                }

    #define APP_TRACE(...)  ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define APP_INFO(...)   ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
    #define APP_WARN(...)   ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define APP_ERROR(...)  ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
    #define APP_FATAL(...)  ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__); \
                            DEBUGBREAK

    #define APP_ASSERT(x, msg) if (!x) { \
                                APP_FATAL("Assertion failed: {0}", msg); \
                                }
#else
    #define CORE_TRACE(...)
    #define CORE_INFO(...)
    #define CORE_WARN(...)
    #define CORE_ERROR(...)
    #define CORE_FATAL(...)

    #define APP_TRACE(...)
    #define APP_INFO(...)
    #define APP_WARN(...)
    #define APP_ERROR(...)
    #define APP_FATAL(...)
#endif