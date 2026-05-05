#pragma once

#include "BlockRegistry.h"
#include "MeshBuilder.h"
#include "opticrafter/OptiCrafter.h"

class Chunk;

class ChunkMesher {
   public:
    ChunkMesher(const opticrafter::TextureAtlas& atlas, const BlockRegistry& registry)
        : m_atlas(atlas), m_registry(registry) {}

    void build(const Chunk& chunk);

    [[nodiscard]] auto mesh() const { return m_mesh.get(); }

   private:
    const opticrafter::TextureAtlas& m_atlas;
    const BlockRegistry& m_registry;
    MeshBuilder m_mesh_builder;
    std::unique_ptr<opticrafter::Mesh> m_mesh;

    void buildFace(const Chunk& chunk, const glm::vec3 pos, MeshBuilder::Face face);
};