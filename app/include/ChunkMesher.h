#pragma once

#include "BlockRegistry.h"
#include "ChunkGrid.h"
#include "MeshBuilder.h"
#include "opticrafter/OptiCrafter.h"

class Chunk;

struct ChunkRenderData {
    std::unique_ptr<opticrafter::Mesh> mesh;
    opticrafter::AABB aabb;
    glm::mat4 transform;
};

class ChunkMesher {
   public:
    explicit ChunkMesher(const BlockRegistry& registry, const ChunkGrid& chunk_grid)
        : m_registry(registry), m_chunk_grid(chunk_grid) {
        m_builder.reset();
    }

    ChunkMesher(const ChunkMesher&) = delete;
    ChunkMesher& operator=(const ChunkMesher&) = delete;

    void reset();
    MeshData build(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                   std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl);
    static ChunkRenderData uploadToGPU(MeshData&& data);

   private:
    const BlockRegistry& m_registry;
    const ChunkGrid& m_chunk_grid;
    MeshBuilder m_builder;

    std::array<int, 4> computeAO(const glm::ivec3& pos, const glm::ivec3 normal, const glm::ivec3& t,
                                   const glm::ivec3& bt);
};