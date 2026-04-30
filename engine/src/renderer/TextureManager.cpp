#include "opticrafter/TextureManager.h"

#include "opticrafter/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <stb_image.h>

namespace opticrafter {

static GLuint upload(int w, int h, const void* pixels) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}

TextureManager::TextureManager(const GLContext& ctx) {
    (void)ctx;  // Guard, need a valid context to build texture manager
}

TextureManager::~TextureManager() { glDeleteTextures(m_textures.size(), m_textures.data()); }

TextureID TextureManager::loadFromFile(const std::string& path) {
    int w, h, channels;
    unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &channels, 4);
    if (!pixels) {
        LOG_CORE_ERROR("Failed to load texture '%s': %s", path, stbi_failure_reason());
    }
    GLuint handle = upload(w, h, pixels);
    stbi_image_free(pixels);

    m_textures.push_back(handle);

    LOG_CORE_TRACE("Texture loaded: {} ({}x{})", path, w, h);

    return m_textures.size() - 1;
}

TextureID TextureManager::loadFromData(uint32_t width, uint32_t height, std::span<const std::byte> data) {
    GLuint handle = upload(width, height, data.data());

    m_textures.push_back(handle);

    LOG_CORE_TRACE("Texture loaded from data ({}x{})", width, height);

    return m_textures.size() - 1;
}

void TextureManager::unload(TextureID id) {
    if (id >= m_textures.size()) {
        LOG_CORE_ERROR("Texture {} doesn't exists.");
        return;
    }

    glDeleteTextures(1, &m_textures[id]);
}

void TextureManager::bind(TextureID id, uint32_t slot) {
    if (id >= m_textures.size()) {
        LOG_CORE_ERROR("Texture {} doesn't exists.");
        return;
    }

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textures[id]);
}

GLuint TextureManager::handle(TextureID id) const {
    if (id >= m_textures.size()) {
        LOG_CORE_ERROR("Texture {} doesn't exists.");
        return 0;
    }

    return m_textures.at(id);
}

}  // namespace opticrafter