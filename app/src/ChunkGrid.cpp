#include "ChunkGrid.h"

ChunkGrid::ChunkGrid(uint8_t render_distance, glm::ivec2 origin) : m_origin(origin), m_size(render_distance * 2 + 1) {
    m_chunks.resize(m_size * m_size);

    for (int z = -render_distance; z <= render_distance; z++) {
        for (int x = -render_distance; x <= render_distance; x++) {
            auto i = index(x, z);
            m_chunks[i] = std::make_unique<Chunk>(x, z);
        }
    }
}

Chunk* ChunkGrid::getChunk(int cx, int cz) const {
    if (!isInBounds(cx, cz)) {
        return nullptr;
    }

    return m_chunks[index(cx, cz)].get();
}

bool ChunkGrid::isInBounds(int cx, int cz) const {
    int render_distance = (m_size - 1) / 2;
    return cx >= -render_distance && cx <= render_distance && cz >= -render_distance && cz <= render_distance;
}

size_t ChunkGrid::index(int cx, int cz) const {
    int half = m_size / 2;
    int x = cx + half;
    int z = cz + half;
    return x + z * m_size;
}