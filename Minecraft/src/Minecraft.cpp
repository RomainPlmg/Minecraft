#include "Minecraft.hpp"

#include "Block.hpp"

Minecraft::Minecraft() {
    Engine::ShaderLibrary::Create("default",
                                  PROJECT_SOURCE_DIR "Minecraft/assets/shaders/default.vert",
                                  PROJECT_SOURCE_DIR "Minecraft/assets/shaders/default.frag");

    m_TextureAtlases["blockAtlas"] =
        Engine::TextureAtlas::Create(PROJECT_SOURCE_DIR "Minecraft/assets/atlases/blocks.json");
    m_TextureAtlases["blockAtlas"]->Init();

    m_Block = new Block();
}
