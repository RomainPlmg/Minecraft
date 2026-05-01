#pragma once

#include <array>

#include "BlockRegistry.h"

class Chunk {
   public:
    static constexpr uint8_t CHUNK_WIDTH = 16;
    static constexpr uint8_t CHUNK_HEIGHT = 64;

    Chunk();

    [[nodiscard]] BlockType getBlock(uint8_t x, uint8_t y, uint8_t z) const;
    void setBlock(uint8_t x, uint8_t y, uint8_t z, BlockType type);

    auto begin() const { return m_blocks.begin(); }
    auto end() const { return m_blocks.end(); }

   private:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> m_blocks;

    size_t index(uint8_t x, uint8_t y, uint8_t z) const { return x + CHUNK_WIDTH * (y + CHUNK_WIDTH * z); }
};