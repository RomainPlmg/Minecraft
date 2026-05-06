#include "ChunkGrid.h"

ChunkGrid::ChunkGrid(uint8_t render_distance) : m_size(render_distance * 2 + 1) {
    m_chunks.resize(m_size * m_size);

    for (int z = -render_distance; z <= render_distance; z++) {
        for (int x = -render_distance; x <= render_distance; x++) {
            auto i = index(x, z);
            m_chunks[i] = std::make_unique<Chunk>(x, z);
        }
    }
}

std::optional<BlockType> ChunkGrid::getBlock(const glm::ivec3 coord) const {
    // Outside vertical limits
    if (coord.y < 0 || coord.y >= (int)Chunk::CHUNK_HEIGHT) {
        return std::nullopt;
    }

    // Calculate chunk position
    int cx = std::floor(coord.x >> 4);
    int cz = std::floor(coord.z >> 4);

    auto idx = index(cx, cz);
    if (idx >= m_chunks.size()) {
        return std::nullopt;
    }

    // Local coordinates in the chunk
    int lx = coord.x - cx * Chunk::CHUNK_WIDTH;
    int lz = coord.z - cz * Chunk::CHUNK_WIDTH;

    return m_chunks[idx]->getBlock(lx, coord.y, lz);
}

size_t ChunkGrid::index(int cx, int cz) const {
    int x = ((cx % m_size) + m_size) % m_size;  // Modulo to wrap negative coord into [0, size]
    int z = ((cz % m_size) + m_size) % m_size;
    return x + z * m_size;
}