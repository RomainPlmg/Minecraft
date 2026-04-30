#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <span>
#include <string>

namespace opticrafter {

class Renderer;

using TextureID = uint32_t;

struct Region {
    glm::vec2 uv_min;
    glm::vec2 uv_max;
};

class TextureAtlas {
   public:
    TextureAtlas(uint32_t tile_size) : m_tile_size(tile_size) {}

    void add(const std::string& name, const std::string& path);
    void build(Renderer& renderer);
    Region region(const std::string& name) const;
    TextureID handle() const { return m_id; }

   private:
    TextureID m_id = 0;
    uint32_t m_tile_size = 0;
    std::unordered_map<std::string, std::string> m_sources;
    std::unordered_map<std::string, Region> m_regions;

    void blit(std::span<uint8_t> dst, int dst_w, const std::string& path, const glm::ivec2& origin);
};

}  // namespace opticrafter