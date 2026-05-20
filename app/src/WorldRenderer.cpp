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

void WorldRenderer::update(ChunkGrid& grid) {
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

        if (!grid.isInBounds(coords.x, coords.y)) {
            it = m_chunks_to_waiting_neighbors.erase(it);
            continue;
        }

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
        m_engine.threadPool()->enqueue([this, chunk, nf, nb, nr, nl] {
            thread_local ChunkMesher mesher(m_registry);
            m_ready_meshes.push(mesher.build(chunk, nf, nb, nr, nl));
        });

        it = m_chunks_to_mesh.erase(it);
    }

    while (auto mesh_opt = m_ready_meshes.tryPop()) {
        auto mesh = *mesh_opt;

        auto chunk = grid.getChunk(mesh.coords.x, mesh.coords.z);

        if (chunk && chunk->state() == ChunkState::Meshing) {
            const auto coords = chunk->coords();
            m_render_data.set(coords.x, coords.y, m_mesher.uploadToGPU(std::move(mesh)));
            chunk->setState(ChunkState::Meshed);
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