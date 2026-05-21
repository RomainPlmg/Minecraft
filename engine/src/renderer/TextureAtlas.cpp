#include "opticrafter/TextureAtlas.h"

#include <glad/gl.h>
#include <stb_image.h>

#include <filesystem>

#include "opticrafter/Logger.h"

namespace opticrafter {

TextureAtlas::~TextureAtlas() {}

void TextureAtlas::loadTextures(const std::vector<std::string>& texture_paths) {
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_id);
    glTextureStorage3D(m_id, 1, GL_RGBA8, m_tile_size, m_tile_size, texture_paths.size());

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channel;
    int layer = 0;

    stbi_set_flip_vertically_on_load(true);
    for (const auto& path : texture_paths) {
        std::filesystem::path p(path);
        std::string texture_name = p.stem().string();
        m_layer_registry[texture_name] = layer;

        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 4);
        glTextureSubImage3D(m_id, 0, 0, 0, layer++, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
}

void TextureAtlas::bind(uint32_t slot) const {
    if (!m_id) {
        LOG_CORE_ERROR("Unknow texture id {}.", m_id);
    }
    glBindTextureUnit(slot, m_id);
}

std::optional<TextureID> TextureAtlas::get(const std::string& texture_name) {
    if (!m_layer_registry.contains(texture_name)) {
        LOG_CORE_ERROR("Unknow texture {}.", texture_name);
        return std::nullopt;
    }

    return m_layer_registry[texture_name];
}

}  // namespace opticrafter