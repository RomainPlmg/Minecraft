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
target_link_libraries(Minecraft PRIVATE opticrafter)

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