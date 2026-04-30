#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace opticrafter {

class GLContext;

using TextureID = uint32_t;

class TextureManager {
   public:
    explicit TextureManager(const GLContext& ctx);
    ~TextureManager();

    TextureID loadFromFile(const std::string& path);
    TextureID loadFromData(uint32_t width, uint32_t height, std::span<const std::byte> data);
    void unload(TextureID id);
    void bind(TextureID id, uint32_t slot = 0);
    uint32_t handle(TextureID id) const;

   private:
    std::vector<uint32_t> m_textures;
};

}  // namespace opticrafter