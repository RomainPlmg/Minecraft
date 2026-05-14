#include "ChunkGrid.h"

#include "TerrainGenerator.h"

// Increase render_distance by one to solve the neighbor checking while remeshing chunks at world boundary.
// This avoids the special case where a chunk has no neighbors
ChunkGrid::ChunkGrid(opticrafter::ThreadPool& thread_pool, uint8_t build_distance, int ox, int oz)
    : m_ox(0xFFFF), m_oz(0xFFFF), m_size((build_distance + 1) * 2 + 1), m_chunks(m_size) {
    setOrigin(thread_pool, ox, oz);
}

std::shared_ptr<Chunk> ChunkGrid::getChunk(int cx, int cz) const {
    // Lock any write operation on the grid object, but allow read operations
    std::shared_lock lock(m_mutex);
    return getChunkNoLock(cx, cz);
}

bool ChunkGrid::isInBounds(int cx, int cz) const {
    int half = m_size / 2;
    return cx >= m_ox - half && cx <= m_ox + half && cz >= m_oz - half && cz <= m_oz + half;
}

void ChunkGrid::setOrigin(opticrafter::ThreadPool& thread_pool, int ox, int oz) {
    // Lock the grid object
    std::unique_lock lock(m_mutex);
    if (ox == m_ox && oz == m_oz) return;

    int bd = (m_size - 1) / 2;  // Calculate build_distance

    // Check what is outside the new world window
    for (int z = m_oz - bd; z <= m_oz + bd; ++z) {
        for (int x = m_ox - bd; x <= m_ox + bd; ++x) {
            // If old coords are outside the new world window, invalidates the chunk and push it into the invalidated
            // queue
            if (x < ox - bd || x > ox + bd || z < oz - bd || z > oz + bd) {
                auto& slot = m_chunks.get(x, z);
                slot.reset();
                m_invalidated.push({x, z});
            }
        }
    }

    // Update the world origin
    m_ox = ox;
    m_oz = oz;

    // Fill the new chunks
    for (int z = m_oz - bd; z <= m_oz + bd; ++z) {
        for (int x = m_ox - bd; x <= m_ox + bd; ++x) {
            auto& slot = m_chunks.get(x, z);

            // If the chunk does not exists, submit a chunk build task to a thread and recover it's future
            if (!slot || slot->coords().x != x || slot->coords().y != z) {
                m_chunks_to_build.emplace_back(thread_pool.enqueue([x, z] {
                    TerrainGenerator generator;
                    return std::make_shared<Chunk>(x, z, generator);
                }));
            }
        }
    }
}

std::vector<std::shared_ptr<Chunk>> ChunkGrid::pollChunksToMesh(size_t max) {
    std::unique_lock lock(m_mutex);
    std::vector<std::shared_ptr<Chunk>> v;

    size_t i = 0;

    while (!m_dirty.empty() && i < max) {
        v.push_back(m_dirty.front());
        m_dirty.pop();
        i++;
    }

    return v;
}

std::vector<glm::ivec2> ChunkGrid::pollInvalidatedChunks() {
    // Lock the grid object
    std::unique_lock lock(m_mutex);
    std::vector<glm::ivec2> v;

    while (!m_invalidated.empty()) {
        v.push_back(m_invalidated.front());
        m_invalidated.pop();
    }

    return v;
}

void ChunkGrid::pollPendingChunks() {
    // Lock the grid objec
    std::unique_lock lock(m_mutex);

    auto it = m_chunks_to_build.begin();
    while (it != m_chunks_to_build.end()) {
        // Check if there is ready futures
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            // If the future is ready, recover the chunk and push it into the dirty queue
            auto chunk = it->get();
            auto coords = chunk->coords();

            // If the player has moved away in the meantime, ignore it.
            if (isInBounds(coords.x, coords.y)) {
                chunk->setState(ChunkState::Dirty);
                m_chunks.get(coords.x, coords.y) = std::move(chunk);
                m_dirty.push(m_chunks.get(coords.x, coords.y));
            }

            it = m_chunks_to_build.erase(it);
        } else {
            // If the current future is not ready, take the next one
            it++;
        }
    }
}

std::shared_ptr<Chunk> ChunkGrid::getChunkNoLock(int cx, int cz) const {
    if (!isInBounds(cx, cz)) {
        return nullptr;
    }

    return m_chunks.get(cx, cz);
}

bool ChunkGrid::isBoundary(int cx, int cz) const {
    std::shared_lock lock(m_mutex);
    int rd = (m_size - 1) / 2;  // Calculate render_distance

    int min_x = m_ox - rd;
    int max_x = m_ox + rd;
    int min_z = m_oz - rd;
    int max_z = m_oz + rd;

    return (cx == min_x || cx == max_x || cz == min_z || cz == max_z);
}