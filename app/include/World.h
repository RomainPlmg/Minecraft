#pragma once

#include "BlockRegistry.h"
#include "ChunkGrid.h"
#include "ChunkMesher.h"
#include "WorldRenderer.h"
#include "opticrafter/OptiCrafter.h"

class World {
   public:
    World(opticrafter::Engine& engine);

    void init();
    void update(float dt, const glm::vec3& pos);
    void render(const opticrafter::Frustum& frustum);

   private:
    opticrafter::Engine& m_engine;
    uint8_t m_render_distance = 4;
    BlockRegistry m_registry;
    opticrafter::TextureAtlas m_atlas;

    ChunkGrid m_chunk_grid;
    WorldRenderer m_renderer;
};