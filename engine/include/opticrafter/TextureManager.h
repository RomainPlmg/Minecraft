#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <string>
#include <vector>

namespace opticrafter {

class GLContext;

using TextureID = uint32_t;

class TextureManager {
   public:
    explicit TextureManager(const GLContext& ctx);

    TextureID load(const std::string& path);
    void unload(TextureID id);
    void bind(TextureID id, uint32_t slot = 0);
    GLuint getHandle(TextureID id) const;

   private:
    std::vector<GLuint> m_textures;
};

}  // namespace opticrafter