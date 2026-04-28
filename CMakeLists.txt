cmake_minimum_required(VERSION 3.30.0)
project(Minecraft
        VERSION 0.1.0
        DESCRIPTION "A optimized Minecraft clone"
        LANGUAGES C CXX)

include(FetchContent)

# ============================================================================
# C++ Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_command.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(engine)
add_subdirectory(app)
