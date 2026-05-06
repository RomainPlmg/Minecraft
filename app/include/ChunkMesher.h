#pragma once

#include "BlockRegistry.h"
#include "MeshBuilder.h"
#include "opticrafter/OptiCrafter.h"

class Chunk;
class ChunkGrid;

struct ChunkRenderData {
    std::unique_ptr<opticrafter::Mesh> mesh;
    glm::mat4 transform;
};

class ChunkMesher {
   public:
    ChunkMesher(const opticrafter::TextureAtlas& atlas, const BlockRegistry& registry)
        : m_atlas(atlas), m_registry(registry) {}

    ChunkRenderData build(const Chunk& chunk, const ChunkGrid& grid);

   private:
    const opticrafter::TextureAtlas& m_atlas;
    const BlockRegistry& m_registry;
    MeshBuilder m_mesh_builder;

    inline void buildMeshInterior(const Chunk& chunk);
    inline void buildMeshExterior(const Chunk& chunk, const ChunkGrid& grid);
};