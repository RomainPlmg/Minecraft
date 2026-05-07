#pragma once

#include <array>

#include "BlockRegistry.h"

class Chunk {
   public:
    static constexpr int CHUNK_WIDTH = 3;
    static constexpr int CHUNK_HEIGHT = 3;

    Chunk(int cx, int cz);

    [[nodiscard]] BlockType getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockType type);
    bool contains(int x, int y, int z) const;

    glm::ivec2 coords() const { return m_coords; }
    auto begin() const { return m_blocks.begin(); }
    auto end() const { return m_blocks.end(); }

   private:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> m_blocks;
    glm::ivec2 m_coords;

    size_t index(int x, int y, int z) const { return x + CHUNK_WIDTH * y + CHUNK_WIDTH * CHUNK_HEIGHT * z; }
};