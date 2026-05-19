#pragma once

#include <set>

#include "ChunkMesher.h"
#include "opticrafter/OptiCrafter.h"

class ChunkGrid;
class BlockRegistry;

class WorldRenderer {
   public:
    WorldRenderer(opticrafter::Engine& engine, const BlockRegistry& registry, const opticrafter::TextureAtlas& atlas,
                  const ChunkGrid& chunk_grid, int render_distance);
    void update(ChunkGrid& grid, const glm::vec3 coords);
    void render(const opticrafter::Frustum& frustum);

   private:
    opticrafter::Engine& m_engine;
    const BlockRegistry& m_registry;
    const opticrafter::TextureAtlas& m_atlas;
    const ChunkGrid& m_chunk_grid;
    ChunkMesher m_mesher;
    std::set<std::shared_ptr<Chunk>> m_chunks_to_mesh;               // Use set to avoid duplicates
    std::set<std::shared_ptr<Chunk>> m_chunks_to_waiting_neighbors;  // Use set to avoid duplicates
    std::vector<std::future<MeshData>> m_pending_meshes;
    opticrafter::RingBuffer2D<std::optional<ChunkRenderData>> m_render_data;
};