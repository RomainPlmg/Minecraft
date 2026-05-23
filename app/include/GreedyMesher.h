#pragma once

#include "BlockRegistry.h"
#include "Chunk.h"
#include "MeshBuilder.h"

struct GreedyItem {
    bool visible;
    BlockType type;
    glm::ivec3 pos;
    std::array<int, 4> ao;
    bool visited;
};

class GreedyMesher {
   public:
    GreedyMesher(const BlockRegistry& registry) : m_registry(registry) {}

    void run(const std::span<BlockType>& grid, MeshBuilder& builder);

   private:
    const BlockRegistry& m_registry;

    // Masks
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_WIDTH> m_top;
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_WIDTH> m_bottom;
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT> m_right;
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT> m_left;
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT> m_front;
    std::array<GreedyItem, Chunk::CHUNK_WIDTH * Chunk::CHUNK_HEIGHT> m_back;

    void xPass(const std::span<BlockType>& grid, MeshBuilder& builder);
    void yPass(const std::span<BlockType>& grid, MeshBuilder& builder);
    void zPass(const std::span<BlockType>& grid, MeshBuilder& builder);
    std::array<int, 4> computeAO(const std::span<BlockType>& grid, const glm::ivec3& pos, const glm::ivec3 normal,
                                 const glm::ivec3& t, const glm::ivec3& bt);

    inline size_t index(int x, int y, int z) const;
    inline BlockType getBlockLocal(const std::span<BlockType>& grid, int x, int y, int z);
};