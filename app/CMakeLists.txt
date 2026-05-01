# ============================================================================
# Define executable
# ============================================================================
file(GLOB_RECURSE APP_SRC
        "src/*.cpp"
)

add_executable(Minecraft ${APP_SRC})
target_include_directories(Minecraft PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_definitions(Minecraft PRIVATE -DASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

# ============================================================================
# Dependencies
# ============================================================================
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.7
)
FetchContent_MakeAvailable(imgui)

# Create imgui target
file(GLOB IMGUI_SRC
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(Minecraft PRIVATE opticrafter imgui)

# ============================================================================
# Compilation properties
# ============================================================================
target_compile_features(Minecraft PUBLIC cxx_std_20)

# Warnings
if(MSVC)
    target_compile_options(Minecraft PRIVATE /W4)
else()
    target_compile_options(Minecraft PRIVATE -Wall -Wextra -Wpedantic)
endif()