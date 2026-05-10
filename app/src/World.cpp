#include "World.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

World::World(opticrafter::Engine& engine)
    : m_engine(engine),
      m_atlas(32),
      m_chunk_grid(*engine.threadPool(), m_render_distance),
      m_chunk_mesher(m_atlas, m_registry),
      m_chunk_render_data(m_render_distance * 2 + 1) {
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

void World::update(float dt, const glm::vec3& pos) {
    m_chunk_grid.pollPendingChunks();

    m_chunk_grid.setOrigin(*m_engine.threadPool(), std::floor(pos.x / (float)Chunk::CHUNK_WIDTH),
                           std::floor(pos.z / (float)Chunk::CHUNK_WIDTH));

    for (const auto& coord : m_chunk_grid.pollInvalidatedChunks()) {
        m_chunk_render_data.set(coord.x, coord.y, std::nullopt);
    }

    for (auto chunk : m_chunk_grid.pollDirtyChunks(MAX_CHUNK_MESHED_PER_FRAME)) {
        // Before mesh the chunk, check that it was not invalidates in the meantime
        auto* current = m_chunk_grid.getChunk(chunk->coords().x, chunk->coords().y);
        if (current != chunk) continue;

        chunk->setState(ChunkState::Meshing);
        m_pending_meshes.emplace_back(m_engine.threadPool()->enqueue([chunk, this] {
            ChunkMesher mesher(m_atlas, m_registry);
            return mesher.build(*chunk, m_chunk_grid);
        }));
    }

    auto it = m_pending_meshes.begin();
    while (it != m_pending_meshes.end()) {
        // Check if the future is ready
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto mesh = it->get();
            auto* chunk = m_chunk_grid.getChunk(mesh.coords.x, mesh.coords.z);

            if (chunk && chunk->state() == ChunkState::Meshing) {
                m_chunk_render_data.set(mesh.coords.x, mesh.coords.z, m_chunk_mesher.uploadToGPU(std::move(mesh)));
                chunk->setState(ChunkState::Meshed);
            } else if (chunk && chunk->state() == ChunkState::Dirty) {
                chunk->setState(ChunkState::Meshing);
                m_pending_meshes.emplace_back(m_engine.threadPool()->enqueue([chunk, this] {
                    ChunkMesher mesher(m_atlas, m_registry);
                    return mesher.build(*chunk, m_chunk_grid);
                }));
            }

            it = m_pending_meshes.erase(it);
        } else {
            it++;
        }
    }
}

void World::render(const opticrafter::Frustum& frustum) {
    ZoneScopedN("WorldRender");
    TracyGpuZone("Draw chunks");
    m_engine.renderer()->textures()->bind(m_atlas.handle());

    for (auto& data : m_chunk_render_data) {
        if (!data) continue;
        if (!frustum.intersects(data->aabb)) continue;

        m_engine.renderer()->draw(*data->mesh, {0}, data->transform);
    }
}