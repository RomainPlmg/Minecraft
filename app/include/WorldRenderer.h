#pragma once

#include <set>

#include "ChunkMesher.h"
#include "opticrafter/OptiCrafter.h"

class BlockRegistry;

class WorldRenderer {
   public:
    WorldRenderer(opticrafter::Engine& engine, const BlockRegistry& registry, const opticrafter::TextureAtlas& atlas,
                  int render_distance);
    void update(ChunkGrid& grid);
    void render(const opticrafter::Frustum& frustum);

   private:
    opticrafter::Engine& m_engine;
    const BlockRegistry& m_registry;
    const opticrafter::TextureAtlas& m_atlas;
    ChunkMesher m_mesher;
    std::set<std::shared_ptr<Chunk>> m_chunks_to_mesh;               // Use set to avoid duplicates
    std::set<std::shared_ptr<Chunk>> m_chunks_to_waiting_neighbors;  // Use set to avoid duplicates
    opticrafter::TSQueue<MeshData> m_ready_meshes;
    opticrafter::RingBuffer2D<std::optional<ChunkRenderData>> m_render_data;
};