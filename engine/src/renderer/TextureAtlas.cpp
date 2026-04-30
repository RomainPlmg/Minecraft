#include "opticrafter/TextureAtlas.h"

#include <stb_image.h>

#include "opticrafter/Logger.h"
#include "opticrafter/Renderer.h"

namespace opticrafter {

static int nextPow2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

void TextureAtlas::add(const std::string& name, const std::string& path) { m_sources[name] = path; }

void TextureAtlas::build(Renderer& renderer) {
    int n = m_sources.size();
    int cols = (int)std::ceil(std::sqrt(n));
    int rows = (cols * (cols - 1) >= n) ? cols - 1 : cols;

    // Resize the atlas to have a power of 2 (16, 32, 64, 128, etc.) -> Better for the GPU
    int atlas_w = nextPow2(cols * m_tile_size);
    int atlas_h = nextPow2(rows * m_tile_size);

    // Pixel buffer
    std::vector<uint8_t> buffer(atlas_w * atlas_h * 4, 0);

    int i = 0;
    for (auto& [name, path] : m_sources) {
        int x = (i % cols) * m_tile_size;
        int y = (i / cols) * m_tile_size;

        // Load image and copy it into the buffer at (x, y)
        blit(buffer, atlas_w, path, {x, y});

        // Calculate UVs
        m_regions[name] = {
            {(float)x / atlas_w, (float)y / atlas_h},
            {(float)(x + m_tile_size) / atlas_w, (float)(y + m_tile_size) / atlas_h},
        };
        i++;
    }

    m_id = renderer.createTextureFromData(atlas_w, atlas_h, std::as_bytes(std::span{buffer}));
}

void TextureAtlas::blit(std::span<uint8_t> dst, int dst_w, const std::string& path, const glm::ivec2& origin) {
    int w, h, channels;
    uint8_t* src = stbi_load(path.c_str(), &w, &h, &channels, 4);
    if (!src) {
        LOG_CORE_ERROR("Failed to load texture: {}", path);
        return;
    }

    for (int row = 0; row < h; row++) {
        int dst_offset = ((origin.y + row) * dst_w + origin.x) * 4;
        int src_offset = row * w * 4;
        std::memcpy(&dst[dst_offset], src + src_offset, w * 4);
    }

    stbi_image_free(src);
}

}  // namespace opticrafter