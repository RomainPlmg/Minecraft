#include "ChunkGrid.h"

#include "TerrainGenerator.h"

ChunkGrid::ChunkGrid(opticrafter::ThreadPool& thread_pool, uint8_t render_distance, int ox, int oz)
    : m_ox(0xFFFF), m_oz(0xFFFF), m_size(render_distance * 2 + 1), m_chunks(m_size) {
    setOrigin(thread_pool, ox, oz);
}

Chunk* ChunkGrid::getChunk(int cx, int cz) const {
    std::shared_lock lock(m_mutex);
    return getChunkNoLock(cx, cz);
}

bool ChunkGrid::isInBounds(int cx, int cz) const {
    int half = m_size / 2;
    return cx >= m_ox - half && cx <= m_ox + half && cz >= m_oz - half && cz <= m_oz + half;
}

void ChunkGrid::setOrigin(opticrafter::ThreadPool& thread_pool, int ox, int oz) {
    std::unique_lock lock(m_mutex);
    if (ox == m_ox && oz == m_oz) return;

    int rd = (m_size - 1) / 2;

    // Check what is outside the new world window
    for (int z = m_oz - rd; z <= m_oz + rd; ++z) {
        for (int x = m_ox - rd; x <= m_ox + rd; ++x) {
            // If old coord is outside the new world window
            if (x < ox - rd || x > ox + rd || z < oz - rd || z > oz + rd) {
                auto& slot = m_chunks.get(x, z);
                slot.reset();
                m_invalidated.push({x, z});
            }
        }
    }

    // Update origin
    m_ox = ox;
    m_oz = oz;

    // Fill new chunks
    for (int z = m_oz - rd; z <= m_oz + rd; ++z) {
        for (int x = m_ox - rd; x <= m_ox + rd; ++x) {
            auto& slot = m_chunks.get(x, z);

            if (!slot || slot->coords().x != x || slot->coords().y != z) {
                m_chunks_to_build.emplace_back(thread_pool.enqueue([x, z] {
                    TerrainGenerator generator;
                    return std::make_unique<Chunk>(x, z, generator);
                }));

                // Invalidates neighbors
                auto* neighbor = getChunkNoLock(x, z + 1);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunkNoLock(x, z - 1);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunkNoLock(x + 1, z);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunkNoLock(x - 1, z);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }
            }
        }
    }
}

std::vector<Chunk*> ChunkGrid::pollDirtyChunks(size_t max) {
    std::unique_lock lock(m_mutex);
    std::vector<Chunk*> v;

    size_t i = 0;
    while (!m_dirty.empty() && i < max) {
        v.push_back(m_dirty.front());
        m_dirty.pop();
        i++;
    }

    return v;
}

std::vector<glm::ivec2> ChunkGrid::pollInvalidatedChunks() {
    std::unique_lock lock(m_mutex);
    std::vector<glm::ivec2> v;

    while (!m_invalidated.empty()) {
        v.push_back(m_invalidated.front());
        m_invalidated.pop();
    }

    return v;
}

void ChunkGrid::pollPendingChunks() {
    std::unique_lock lock(m_mutex);
    auto it = m_chunks_to_build.begin();
    while (it != m_chunks_to_build.end()) {
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto chunk = it->get();
            auto coords = chunk->coords();
            chunk->setState(ChunkState::Dirty);
            m_chunks.get(coords.x, coords.y) = std::move(chunk);
            m_dirty.push(m_chunks.get(coords.x, coords.y).get());

            auto* neighbor = getChunkNoLock(coords.x, coords.y + 1);
            if (neighbor && neighbor->state() == ChunkState::Meshed) {
                neighbor->setState(ChunkState::Dirty);
                m_dirty.push(neighbor);
            }
            neighbor = getChunkNoLock(coords.x, coords.y - 1);
            if (neighbor && neighbor->state() == ChunkState::Meshed) {
                neighbor->setState(ChunkState::Dirty);
                m_dirty.push(neighbor);
            }
            neighbor = getChunkNoLock(coords.x + 1, coords.y);
            if (neighbor && neighbor->state() == ChunkState::Meshed) {
                neighbor->setState(ChunkState::Dirty);
                m_dirty.push(neighbor);
            }
            neighbor = getChunkNoLock(coords.x - 1, coords.y);
            if (neighbor && neighbor->state() == ChunkState::Meshed) {
                neighbor->setState(ChunkState::Dirty);
                m_dirty.push(neighbor);
            }

            it = m_chunks_to_build.erase(it);
        } else {
            it++;
        }
    }
}

Chunk* ChunkGrid::getChunkNoLock(int cx, int cz) const {
    if (!isInBounds(cx, cz)) {
        return nullptr;
    }

    return m_chunks.get(cx, cz).get();
}