#include "Block.hpp"

Block::Block() {
    m_Shader = Engine::ShaderLibrary::GetShader("default");
    m_Texture =
        Engine::Application::GetInstance()->GetTextureAtlas("blockAtlas")->GetTexture("stone");

    Engine::TextureInfo::UV uv = m_Texture->GetAtlasUV();

    // clang-format off
     m_Vertices = {
                   -0.5f, -0.5f, -5.0f,   0.0f, 0.0f, 0.0f,   uv.uMin, uv.vMin,
                   -0.5f,  0.5f, -5.0f,   0.0f, 0.0f, 0.0f,   uv.uMin, uv.vMax,
                    0.5f,  0.5f, -5.0f,   0.0f, 0.0f, 0.0f,   uv.uMax, uv.vMax,
                    0.5f, -0.5f, -5.0f,   0.0f, 0.0f, 0.0f,   uv.uMax, uv.vMin
     };
    // clang-format on

    Register();
}
