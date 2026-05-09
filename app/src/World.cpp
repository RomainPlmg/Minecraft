#include "World.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

World::World(opticrafter::Renderer& renderer)
    : m_renderer(renderer),
      m_atlas(32),
      m_chunk_grid(m_render_distance),
      m_chunk_mesher(m_atlas, m_registry),
      m_chunk_render_data(m_render_distance * 2 + 1) {}

void World::init() {
    ZoneScopedN("WorldInit");

    // Build the texture atlas
    m_atlas.add("stone", ASSETS_DIR "textures/stone.png");
    m_atlas.add("dirt", ASSETS_DIR "textures/dirt.png");
    m_atlas.add("grass_block_side", ASSETS_DIR "textures/grass_block_side.png");
    m_atlas.add("grass_block_top", ASSETS_DIR "textures/grass_block_top.png");
    m_atlas.add("copper_block", ASSETS_DIR "textures/copper_block.png");
    m_atlas.build(m_renderer);

    m_registry.registerBlock(BlockType::AIR, {
                                                 .name = "air",
                                                 .transparent = true,
                                             });
    m_registry.registerBlock(BlockType::GRASS, {
                                                   .name = "grass",
                                                   .top = m_atlas.region("grass_block_top"),
                                                   .side = m_atlas.region("grass_block_side"),
                                                   .bottom = m_atlas.region("dirt"),
                                                   .transparent = false,
                                               });
    m_registry.registerBlock(BlockType::DIRT, {
                                                  .name = "dirt",
                                                  .top = m_atlas.region("dirt"),
                                                  .side = m_atlas.region("dirt"),
                                                  .bottom = m_atlas.region("dirt"),
                                                  .transparent = false,
                                              });
    m_registry.registerBlock(BlockType::STONE, {
                                                   .name = "stone",
                                                   .top = m_atlas.region("stone"),
                                                   .side = m_atlas.region("stone"),
                                                   .bottom = m_atlas.region("stone"),
                                                   .transparent = false,
                                               });
    m_registry.registerBlock(BlockType::COPPER_BLOCK, {
                                                          .name = "copper_block",
                                                          .top = m_atlas.region("copper_block"),
                                                          .side = m_atlas.region("copper_block"),
                                                          .bottom = m_atlas.region("copper_block"),
                                                          .transparent = false,
                                                      });
    for (const auto& chunk : m_chunk_grid) {
        auto coords = chunk->coords();
        auto mesh_data = m_chunk_mesher.build(*chunk, m_chunk_grid);
        m_chunk_render_data.set(coords.x, coords.y, m_chunk_mesher.uploadToGPU(std::move(mesh_data)));
    }
}

void World::update(float dt, const glm::vec3& pos) {
    m_chunk_grid.setOrigin(std::floor(pos.x / (float)Chunk::CHUNK_WIDTH),
                           std::floor(pos.z / (float)Chunk::CHUNK_WIDTH));

    for (const auto& coord : m_chunk_grid.pollInvalidatedChunks()) {
        m_chunk_render_data.set(coord.x, coord.y, std::nullopt);
    }

    for (auto chunk : m_chunk_grid.pollDirtyChunks(MAX_CHUNK_MESHED_PER_FRAME)) {
        // Before mesh the chunk, check that it was not invalidates in the meantime
        auto* current = m_chunk_grid.getChunk(chunk->coords().x, chunk->coords().y);
        if (current != chunk) continue;

        auto coords = chunk->coords();
        auto mesh_data = m_chunk_mesher.build(*chunk, m_chunk_grid);
        m_chunk_render_data.set(coords.x, coords.y, m_chunk_mesher.uploadToGPU(std::move(mesh_data)));
        chunk->setState(ChunkState::Meshed);
    }
}

void World::render(const opticrafter::Frustum& frustum) {
    ZoneScopedN("WorldRender");
    TracyGpuZone("Draw chunks");
    m_renderer.textures()->bind(m_atlas.handle());

    for (auto& data : m_chunk_render_data) {
        if (!data) continue;
        if (!frustum.intersects(data->aabb)) continue;

        m_renderer.draw(*data->mesh, {0}, data->transform);
    }
}