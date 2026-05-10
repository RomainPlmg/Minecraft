#pragma once

#include "BlockRegistry.h"
#include "ChunkGrid.h"
#include "ChunkMesher.h"
#include "opticrafter/OptiCrafter.h"

constexpr int MAX_CHUNK_MESHED_PER_FRAME = 1;

class World {
   public:
    World(opticrafter::Engine& engine);

    void init();
    void update(float dt, const glm::vec3& pos);
    void render(const opticrafter::Frustum& frustum);

   private:
    uint8_t m_render_distance = 16;
    opticrafter::Engine& m_engine;
    opticrafter::TextureAtlas m_atlas;
    BlockRegistry m_registry;
    ChunkGrid m_chunk_grid;
    ChunkMesher m_chunk_mesher;
    std::vector<std::future<MeshData>> m_pending_meshes;

    opticrafter::RingBuffer2D<std::optional<ChunkRenderData>> m_chunk_render_data;
};