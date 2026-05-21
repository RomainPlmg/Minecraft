#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <string>

namespace opticrafter {

class Renderer;

using TextureID = uint32_t;

struct UVRegion {
    glm::vec2 uv_min;
    glm::vec2 uv_max;
};

class TextureAtlas {
   public:
    TextureAtlas(uint32_t tile_size) : m_tile_size(tile_size) {}
    ~TextureAtlas();

    void loadTextures(const std::vector<std::string>& texture_paths);
    void bind(uint32_t slot = 0) const;
    std::optional<TextureID> get(const std::string& texture_name);

   private:
    uint32_t m_tile_size = 0;
    TextureID m_id = 0;
    std::unordered_map<std::string, TextureID> m_layer_registry;
};

}  // namespace opticrafter