#include "OpenGLTextureAtlas.hpp"

#include <glm/glm.hpp>

#include "Engine/Renderer/Texture.hpp"
#include "OpenGLContext.hpp"

namespace Engine {

OpenGLTextureAtlas::OpenGLTextureAtlas() {
    LoadJSON(PROJECT_SOURCE_DIR "Minecraft/assets/atlases/blocks.json");
}

void OpenGLTextureAtlas::Init() {
    // Create the atlas texture
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 nullptr);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glm::vec2 offset = glm::vec2(0);
    for (const auto& texture : m_TextureArray) {
        if (offset.x >= m_Width) {
            offset.y += 16;
            offset.x = 0;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<int>(offset.x), static_cast<int>(offset.y),
                        texture.second->GetWidth(), texture.second->GetHeight(), GL_RGBA,
                        GL_UNSIGNED_BYTE, texture.second->GetData());

        // Calculate the UV coordinates of the texture in the atlas
        float uMin = offset.x / static_cast<float>(m_Width);
        float vMin = offset.y / static_cast<float>(m_Height);
        float uMax =
            static_cast<float>(offset.x + texture.second->GetWidth()) / static_cast<float>(m_Width);
        float vMax = static_cast<float>(offset.y + texture.second->GetHeight()) /
                     static_cast<float>(m_Height);
        texture.second->SetAtlasUV({uMin, uMax, vMin, vMax});

        texture.second->CleanTextureData();

        offset.x += 16;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    Bind();
}

void OpenGLTextureAtlas::Bind() { glBindTexture(GL_TEXTURE_2D, m_RendererID); }

}  // namespace Engine
