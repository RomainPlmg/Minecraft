# ============================================================================
# Define library
# ============================================================================
file(GLOB_RECURSE CRAFTER_SRC
        "src/*.cpp"
)

add_library(opticrafter STATIC ${CRAFTER_SRC})

# Alias
add_library(opticrafter::opticrafter ALIAS opticrafter)
target_include_directories(opticrafter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ============================================================================
# Options
# ============================================================================
option(TRACY_PROFILER "Enable Tracy profiler" ON)

# ============================================================================
# Dependencies
# ============================================================================
# OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(vendor)

# SDL3
find_package(SDL3 3.4.0 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, download via FetchContent...")
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.4.0
    )
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL3)
endif ()

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

if(TRACY_PROFILER)
        set(TRACY_ENABLE ON CACHE BOOL "" FORCE)
        set(TRACY_OPENGL ON CACHE BOOL "" FORCE)
else ()
        set(TRACY_ENABLE OFF CACHE BOOL "" FORCE)
        set(TRACY_OPENGL OFF CACHE BOOL "" FORCE)
        message(STATUS "Tracy profiler is disabled.")
endif()

FetchContent_MakeAvailable (tracy)
target_link_libraries(opticrafter PUBLIC TracyClient)

target_link_libraries(opticrafter PUBLIC OpenGL::GL SDL3::SDL3 spdlog glad stb_image glm::glm)

# ============================================================================
# Compilation properties
# ============================================================================
target_compile_features(opticrafter PUBLIC cxx_std_20)

# Warnings
if (MSVC)
    target_compile_options(opticrafter PRIVATE /W4)
else ()
    target_compile_options(opticrafter PRIVATE -Wall -Wextra -Wpedantic)
endif ()