#include "World.h"

World::World(opticrafter::Engine& engine)
    : m_engine(engine),
      m_atlas(32),
      m_chunk_grid(*m_engine.threadPool(), m_render_distance),
      m_renderer(engine, m_registry, m_atlas, m_render_distance) {
    // Build the texture atlas
    m_atlas.loadTextures({
        ASSETS_DIR "textures/stone.png",
        ASSETS_DIR "textures/dirt.png",
        ASSETS_DIR "textures/grass_block_side.png",
        ASSETS_DIR "textures/grass_block_top.png",
        ASSETS_DIR "textures/copper_block.png",
        ASSETS_DIR "textures/bedrock.png",
    });

    m_registry.registerBlock(BlockType::AIR, {
                                                 .name = "air",
                                                 .transparent = true,
                                             });
    m_registry.registerBlock(BlockType::GRASS, {
                                                   .name = "grass",
                                                   .top = m_atlas.get("grass_block_top").value(),
                                                   .side = m_atlas.get("grass_block_side").value(),
                                                   .bottom = m_atlas.get("dirt").value(),
                                                   .transparent = false,
                                               });
    m_registry.registerBlock(BlockType::DIRT, {
                                                  .name = "dirt",
                                                  .top = m_atlas.get("dirt").value(),
                                                  .side = m_atlas.get("dirt").value(),
                                                  .bottom = m_atlas.get("dirt").value(),
                                                  .transparent = false,
                                              });
    m_registry.registerBlock(BlockType::STONE, {
                                                   .name = "stone",
                                                   .top = m_atlas.get("stone").value(),
                                                   .side = m_atlas.get("stone").value(),
                                                   .bottom = m_atlas.get("stone").value(),
                                                   .transparent = false,
                                               });
    m_registry.registerBlock(BlockType::COPPER_BLOCK, {
                                                          .name = "copper_block",
                                                          .top = m_atlas.get("copper_block").value(),
                                                          .side = m_atlas.get("copper_block").value(),
                                                          .bottom = m_atlas.get("copper_block").value(),
                                                          .transparent = false,
                                                      });
    m_registry.registerBlock(BlockType::BEDROCK, {
                                                     .name = "bedrock",
                                                     .top = m_atlas.get("bedrock").value(),
                                                     .side = m_atlas.get("bedrock").value(),
                                                     .bottom = m_atlas.get("bedrock").value(),
                                                     .transparent = false,
                                                 });
}

void World::init() {}

void World::update(float dt, const glm::vec3& coords) {
    m_chunk_grid.update(*m_engine.threadPool(), std::floor(coords.x / (float)Chunk::CHUNK_WIDTH),
                        std::floor(coords.z / (float)Chunk::CHUNK_WIDTH));
    m_renderer.update(m_chunk_grid);
}

void World::render(const opticrafter::Frustum& frustum) { m_renderer.render(frustum); }