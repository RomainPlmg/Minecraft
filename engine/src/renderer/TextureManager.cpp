#include "opticrafter/TextureManager.h"

#include "opticrafter/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace opticrafter {

static GLuint upload(int w, int h, const void* pixels) {
    GLuint id;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureStorage2D(id, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    return id;
}

TextureManager::TextureManager(const GLContext& ctx) {
    (void)ctx;  // Guard, need a valid context to build texture manager
}

TextureID TextureManager::load(const std::string& path) {
    int w, h, channels;
    unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &channels, 4);
    if (!pixels) {
        LOG_CORE_ERROR("Failed to load texture '%s': %s", path, stbi_failure_reason());
    }
    GLuint handle = upload(w, h, pixels);
    stbi_image_free(pixels);

    m_textures.push_back(handle);

    LOG_CORE_TRACE("Texture loaded: %s (%dx%d)", path, w, h);

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

    glBindTextureUnit(slot, m_textures[id]);
}

GLuint TextureManager::getHandle(TextureID id) const {
    if (id >= m_textures.size()) {
        LOG_CORE_ERROR("Texture {} doesn't exists.");
        return 0;
    }

    return m_textures.at(id);
}

}  // namespace opticrafter