#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace opticrafter {

class GLContext;

using TextureID = uint32_t;

class TextureManager {
   public:
    explicit TextureManager(const GLContext& ctx);
    ~TextureManager();

    TextureID load(const std::string& path);
    void unload(TextureID id);
    void bind(TextureID id, uint32_t slot = 0);
    uint32_t handle(TextureID id) const;

   private:
    std::vector<uint32_t> m_textures;
};

}  // namespace opticrafter