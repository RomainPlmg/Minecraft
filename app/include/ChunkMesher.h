#pragma once

#include "BlockRegistry.h"
#include "MeshBuilder.h"
#include "opticrafter/OptiCrafter.h"

class Chunk;
class ChunkGrid;

struct ChunkRenderData {
    std::unique_ptr<opticrafter::Mesh> mesh;
    opticrafter::AABB aabb;
    glm::mat4 transform;
};

class ChunkMesher {
   public:
    ChunkMesher(const opticrafter::TextureAtlas& atlas, const BlockRegistry& registry)
        : m_atlas(atlas), m_registry(registry) {
        m_builder.reset();
    }

    void reset();
    MeshData build(const Chunk& chunk, const ChunkGrid& grid);
    static ChunkRenderData uploadToGPU(MeshData&& data);

   private:
    const opticrafter::TextureAtlas& m_atlas;
    const BlockRegistry& m_registry;
    MeshBuilder m_builder;
};