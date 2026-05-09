#include "ChunkGrid.h"

ChunkGrid::ChunkGrid(uint8_t render_distance, int ox, int oz)
    : m_ox(0xFFFF), m_oz(0xFFFF), m_size(render_distance * 2 + 1), m_chunks(m_size) {
    setOrigin(ox, oz);
}

Chunk* ChunkGrid::getChunk(int cx, int cz) const {
    if (!isInBounds(cx, cz)) {
        return nullptr;
    }

    return m_chunks.get(cx, cz).get();
}

bool ChunkGrid::isInBounds(int cx, int cz) const {
    int half = m_size / 2;
    return cx >= m_ox - half && cx <= m_ox + half && cz >= m_oz - half && cz <= m_oz + half;
}

void ChunkGrid::setOrigin(int ox, int oz) {
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
                slot = std::make_unique<Chunk>(x, z);
                slot->setState(ChunkState::Dirty);
                m_dirty.push(slot.get());

                // Invalidates neighbors
                auto* neighbor = getChunk(x, z + 1);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunk(x, z - 1);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunk(x + 1, z);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }

                neighbor = getChunk(x - 1, z);
                if (neighbor && neighbor->state() == ChunkState::Meshed) {
                    neighbor->setState(ChunkState::Dirty);
                    m_dirty.push(neighbor);
                }
            }
        }
    }
}

std::vector<Chunk*> ChunkGrid::pollDirtyChunks(size_t max) {
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
    std::vector<glm::ivec2> v;

    while (!m_invalidated.empty()) {
        v.push_back(m_invalidated.front());
        m_invalidated.pop();
    }

    return v;
}