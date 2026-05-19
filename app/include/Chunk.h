#pragma once

#include <array>
#include <shared_mutex>

#include "BlockRegistry.h"

class TerrainGenerator;

enum class ChunkState {
    Empty,
    Generated,
    Meshing,
    Meshed,
    Dirty,
};

class Chunk {
   public:
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 256;

    Chunk(int cx, int cz, const TerrainGenerator& generator);

    [[nodiscard]] std::optional<BlockType> getBlock(int x, int y, int z) const;
    [[nodiscard]] std::optional<BlockType> getBlockNoLock(int x, int y, int z) const;
    [[nodiscard]] ChunkState state() const { return m_state; }
    void setBlock(int x, int y, int z, BlockType type);
    void setState(ChunkState state) { m_state = state; }
    bool contains(int x, int y, int z) const;
    glm::ivec2 coords() const { return m_coords; }

   private:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> m_blocks;
    const glm::ivec2 m_coords;
    std::atomic<ChunkState> m_state = ChunkState::Empty;
    mutable std::shared_mutex m_mutex;

    size_t index(int x, int y, int z) const { return x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z); }
};