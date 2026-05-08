#include "Chunk.h"

Chunk::Chunk(int cx, int cz) : m_coords(cx, cz) {
    m_blocks.fill(BlockType::COPPER_BLOCK);
}

std::optional<BlockType> Chunk::getBlock(int x, int y, int z) const {
    if (!contains(x, y, z)) return std::nullopt;

    return m_blocks[index(x, y, z)];
}

void Chunk::setBlock(int x, int y, int z, BlockType type) { m_blocks[index(x, y, z)] = type; }

bool Chunk::contains(int x, int y, int z) const {
    return x >= 0 && x < CHUNK_WIDTH &&   // X
           y >= 0 && y < CHUNK_HEIGHT &&  // Y
           z >= 0 && z < CHUNK_WIDTH;     // Z
}