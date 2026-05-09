#pragma once

#include "BlockRegistry.h"
#include "ChunkGrid.h"
#include "ChunkMesher.h"
#include "opticrafter/OptiCrafter.h"

constexpr int MAX_CHUNK_MESHED_PER_FRAME = 1;

class World {
   public:
    World(opticrafter::Renderer& renderer);

    void init();
    void update(float dt, const glm::vec3& pos);
    void render(const opticrafter::Frustum& frustum);

   private:
    uint8_t m_render_distance = 8;
    opticrafter::Renderer& m_renderer;
    opticrafter::TextureAtlas m_atlas;
    BlockRegistry m_registry;
    ChunkGrid m_chunk_grid;
    ChunkMesher m_chunk_mesher;

    opticrafter::RingBuffer2D<std::optional<ChunkRenderData>> m_chunk_render_data;
};