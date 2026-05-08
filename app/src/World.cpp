#include "World.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

void World::init() {
    ZoneScoped;

    // Build the texture atlas
    m_atlas.add("stone", ASSETS_DIR "textures/stone.png");
    m_atlas.add("dirt", ASSETS_DIR "textures/dirt.png");
    m_atlas.add("grass_block_side", ASSETS_DIR "textures/grass_block_side.png");
    m_atlas.add("grass_block_top", ASSETS_DIR "textures/grass_block_top.png");
    m_atlas.add("copper_block", ASSETS_DIR "textures/copper_block.png");
    m_atlas.build(m_renderer);

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
        m_chunk_mesher.reset();
        m_chunk_render_data.push_back(m_chunk_mesher.build(*chunk, m_chunk_grid));
    }

    m_renderer.textures()->bind(m_atlas.handle());
}

void World::update(float dt) {}

void World::render(const opticrafter::Frustum& frustum) {
    ZoneScoped;
    TracyGpuZone("Draw chunks");
    for (const auto& data : m_chunk_render_data) {
        if (!frustum.intersects(data.aabb)) continue;

        m_renderer.draw(*data.mesh, {0}, data.transform);
    }
}