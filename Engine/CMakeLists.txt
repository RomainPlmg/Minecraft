# CMakeList.txt : CMake project for the Engine, include source and define
# project specific logic here.
#

# Find OpenGL
find_package(OpenGL REQUIRED)

# Adding all vendor libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_X11 OFF CACHE BOOL "GLFW Wayland only")

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/json)

file(GLOB_RECURSE SRC
        "src/*.hpp"
        "src/*.cpp"
)

# Add source to this project's executable.
add_library (Engine SHARED ${SRC})
set_target_properties(glad PROPERTIES
        POSITION_INDEPENDENT_CODE ON  # For Linux
)

if(WIN32)
        target_compile_definitions(Engine PUBLIC ENG_PLATFORM_WINDOWS)
elseif(UNIX)
        target_compile_definitions(Engine PUBLIC ENG_PLATFORM_LINUX)
endif()

target_compile_definitions(Engine PRIVATE ENG_BUILD_DLL)
target_compile_definitions(Engine PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Engine PROPERTY CXX_STANDARD 20)
endif()

target_precompile_headers(Engine PRIVATE "src/EnginePch.hpp")

# Add include directories
target_include_directories(Engine PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/include/"
)

# Link libraries to the project
target_link_libraries(Engine PRIVATE ${OPENGL_LIBRARY} glfw glad glm)

# Copy the new compiled DLL file to the application runtime directory
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Engine>           # DLL source file
    ${CMAKE_BINARY_DIR}/Minecraft   # Application directory
)