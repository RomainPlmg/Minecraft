# CMakeList.txt : CMake project for KeplerSim, include source and define
# project specific logic here.
#

# Find OpenGL
find_package(OpenGL REQUIRED)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

# Adding all vendor libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

#set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
#set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

#add_subdirectory(vendor/GLFW)
#add_subdirectory(vendor/glad)
#add_subdirectory(vendor/spdlog)
#add_subdirectory(vendor/glm)

file(GLOB_RECURSE SRC
        "src/*.h"
        "src/*.cpp"
)

# Add source to this project's executable.
add_executable (Engine ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Engine PROPERTY CXX_STANDARD 20)
endif()

target_precompile_headers(Engine PUBLIC src/pch.h)

# Add include directories
target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stbimage/
)

# Link libraries to the project
target_link_libraries(Engine PRIVATE ${OPENGL_LIBRARY} glfw glad glm)
