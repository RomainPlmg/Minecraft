#include "WorldRenderer.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

#include "Chunk.h"
#include "ChunkGrid.h"

constexpr int MAX_CHUNK_MESHED_PER_FRAME = 8;

WorldRenderer::WorldRenderer(opticrafter::Engine& engine, const BlockRegistry& registry,
                             const opticrafter::TextureAtlas& atlas, int render_distance)
    : m_engine(engine),
      m_registry(registry),
      m_atlas(atlas),
      m_mesher(registry),
      m_render_data((render_distance + 1) * 2 + 1) {}

void WorldRenderer::update(ChunkGrid& grid, const glm::vec3 coords) {
    grid.setOrigin(*m_engine.threadPool(), std::floor(coords.x / (float)Chunk::CHUNK_WIDTH),
                   std::floor(coords.z / (float)Chunk::CHUNK_WIDTH));

    grid.pollPendingChunks();

    for (const auto& coord : grid.pollInvalidatedChunks()) {
        m_render_data.set(coord.x, coord.y, std::nullopt);
    }

    // Recover the new generated chunks
    for (auto chunk : grid.pollChunksToMesh(MAX_CHUNK_MESHED_PER_FRAME)) {
        const auto coords = chunk->coords();
        // The grid build one more chunk at boundary then the renderer to ensure that all rendered chunks have a valid
        // neighbor
        if (grid.isBoundary(coords.x, coords.y)) {
            m_chunks_to_waiting_neighbors.insert(chunk);
            continue;
        }

        auto nf = grid.getChunk(coords.x, coords.y + 1);
        auto nb = grid.getChunk(coords.x, coords.y - 1);
        auto nr = grid.getChunk(coords.x + 1, coords.y);
        auto nl = grid.getChunk(coords.x - 1, coords.y);

        if (nf && nb && nr && nl)
            m_chunks_to_mesh.insert(chunk);
        else
            m_chunks_to_waiting_neighbors.insert(chunk);
    }

    // Recover chunks waiting for there neighbors
    auto it = m_chunks_to_waiting_neighbors.begin();
    while (it != m_chunks_to_waiting_neighbors.end()) {
        auto chunk = *it;
        const auto coords = chunk->coords();
        auto nf = grid.getChunk(coords.x, coords.y + 1);
        auto nb = grid.getChunk(coords.x, coords.y - 1);
        auto nr = grid.getChunk(coords.x + 1, coords.y);
        auto nl = grid.getChunk(coords.x - 1, coords.y);

        if (nf && nb && nr && nl) {
            m_chunks_to_mesh.insert(chunk);
            it = m_chunks_to_waiting_neighbors.erase(it);
        } else {
            it++;
        }
    }

    it = m_chunks_to_mesh.begin();
    while (it != m_chunks_to_mesh.end()) {
        auto chunk = *it;
        const auto coords = chunk->coords();
        auto nf = grid.getChunk(coords.x, coords.y + 1);
        auto nb = grid.getChunk(coords.x, coords.y - 1);
        auto nr = grid.getChunk(coords.x + 1, coords.y);
        auto nl = grid.getChunk(coords.x - 1, coords.y);

        chunk->setState(ChunkState::Meshing);
        m_pending_meshes.emplace_back(m_engine.threadPool()->enqueue([chunk, nf, nb, nr, nl, this] {
            ChunkMesher mesher(m_registry);
            return mesher.build(chunk, nf, nb, nr, nl);
        }));

        it = m_chunks_to_mesh.erase(it);
    }

    auto itv = m_pending_meshes.begin();
    while (itv != m_pending_meshes.end()) {
        // Check if the future is ready
        if (itv->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto mesh = itv->get();

            auto chunk = grid.getChunk(mesh.coords.x, mesh.coords.z);

            if (chunk) {
                const auto coords = chunk->coords();
                m_render_data.set(coords.x, coords.y, m_mesher.uploadToGPU(std::move(mesh)));
                chunk->setState(ChunkState::Meshed);
            }

            itv = m_pending_meshes.erase(itv);
        } else {
            itv++;
        }
    }
}

void WorldRenderer::render(const opticrafter::Frustum& frustum) {
    ZoneScopedN("WorldRender");
    TracyGpuZone("Draw chunks");
    m_engine.renderer()->textures()->bind(m_atlas.handle());

    for (auto& data : m_render_data) {
        if (!data) continue;
        if (!frustum.intersects(data->aabb)) continue;

        m_engine.renderer()->draw(*data->mesh, {0}, data->transform);
    }
}