#include "World.h"

World::World(opticrafter::Engine& engine)
    : m_engine(engine),
      m_atlas(32),
      m_chunk_grid(*m_engine.threadPool(), m_render_distance),
      m_renderer(engine, m_registry, m_atlas, m_render_distance) {
    // Build the texture atlas
    m_atlas.add("stone", ASSETS_DIR "textures/stone.png");
    m_atlas.add("dirt", ASSETS_DIR "textures/dirt.png");
    m_atlas.add("grass_block_side", ASSETS_DIR "textures/grass_block_side.png");
    m_atlas.add("grass_block_top", ASSETS_DIR "textures/grass_block_top.png");
    m_atlas.add("copper_block", ASSETS_DIR "textures/copper_block.png");
    m_atlas.add("bedrock", ASSETS_DIR "textures/bedrock.png");
    m_atlas.build(*engine.renderer());

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
    m_registry.registerBlock(BlockType::BEDROCK, {
                                                     .name = "bedrock",
                                                     .top = m_atlas.region("bedrock"),
                                                     .side = m_atlas.region("bedrock"),
                                                     .bottom = m_atlas.region("bedrock"),
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