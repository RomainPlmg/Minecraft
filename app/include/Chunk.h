#pragma once

#include <array>

#include "BlockRegistry.h"

enum class ChunkState {
    Empty,
    Generated,
    Meshed,
    Dirty,
};

class Chunk {
   public:
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 256;

    Chunk(int cx, int cz);

    [[nodiscard]] std::optional<BlockType> getBlock(int x, int y, int z) const;
    [[nodiscard]] ChunkState state() const { return m_state; }
    void setBlock(int x, int y, int z, BlockType type);
    void setState(ChunkState state) { m_state = state; }
    bool contains(int x, int y, int z) const;
    glm::ivec2 coords() const { return m_coords; }
    auto begin() const { return m_blocks.begin(); }
    auto end() const { return m_blocks.end(); }

   private:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> m_blocks;
    glm::ivec2 m_coords;
    ChunkState m_state = ChunkState::Empty;

    size_t index(int x, int y, int z) const { return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z); }
};