#include "Texture.hpp"

#include "Engine/Core/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Engine {

Texture::Texture(const std::string& path, const std::string& name) : m_Name(name) {
    // Load image with stb_image
    stbi_set_flip_vertically_on_load(true);  // Flip verticaly images
    m_Data = stbi_load(path.c_str(), &m_TextureInfo.width, &m_TextureInfo.height,
                       &m_TextureInfo.nbChannels, 0);

    if (!m_Data) {
        CORE_ERROR("Failed to load texture: {0}", path);
    }
}

Texture::~Texture() {
    if (m_Data != nullptr) {
        stbi_image_free(m_Data);
    }
}

void Texture::CleanTextureData() {
    stbi_image_free(m_Data);
    m_Data = nullptr;
}

}  // namespace Engine
