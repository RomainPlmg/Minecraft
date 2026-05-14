#pragma once

#include "BlockRegistry.h"
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
    explicit ChunkMesher(const BlockRegistry& registry) : m_registry(registry) { m_builder.reset(); }

    ChunkMesher(const ChunkMesher&) = delete;
    ChunkMesher& operator=(const ChunkMesher&) = delete;

    void reset();
    MeshData build(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                   std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl);
    static ChunkRenderData uploadToGPU(MeshData&& data);

   private:
    const BlockRegistry& m_registry;
    MeshBuilder m_builder;
};