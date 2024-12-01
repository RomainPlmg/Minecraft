#include "Minecraft.hpp"

Minecraft::Minecraft() {
    Engine::ShaderLibrary::Create("default",
                                  PROJECT_SOURCE_DIR "Minecraft/assets/shaders/default.vert",
                                  PROJECT_SOURCE_DIR "Minecraft/assets/shaders/default.frag");
}
