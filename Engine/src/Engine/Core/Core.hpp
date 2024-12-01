#pragma once

// clang-format off
#if defined(ENG_PLATFORM_WINDOWS)
    // Remove useless warnings
    #pragma warning(disable : 4251)
    #ifdef ENG_BUILD_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif // ENG_BUILD_DLL
#elif defined(ENG_PLATFORM_LINUX)
    #ifdef ENG_BUILD_DLL
        #define ENGINE_API __attribute__((visibility("default")))
    #else
        #define ENGINE_API
    #endif // ENG_BUILD_DLL
#else
    #error OS not supported
#endif

#ifdef ENG_DEBUG
    #if defined(ENG_PLATFORM_WINDOWS)
        #define DEBUGBREAK __debugbreak()
    #elif defined(ENG_PLATFORM_LINUX)
        #include "signal.h"
        #define DEBUGBREAK raise(SIGTRAP)
    #endif
#endif // ENG_DEBUG