#pragma once

#include "BlockRegistry.h"
#include "ChunkGrid.h"
#include "ChunkMesher.h"
#include "opticrafter/OptiCrafter.h"

class World {
   public:
    World(opticrafter::Renderer& renderer)
        : m_renderer(renderer), m_atlas(32), m_chunk_grid(4), m_chunk_mesher(m_atlas, m_registry) {}

    void init();
    void update(float dt);
    void render();

   private:
    opticrafter::Renderer& m_renderer;
    opticrafter::TextureAtlas m_atlas;
    std::vector<ChunkRenderData> m_chunk_render_data;
    BlockRegistry m_registry;
    ChunkGrid m_chunk_grid;
    ChunkMesher m_chunk_mesher;
};