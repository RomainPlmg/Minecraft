#include "Chunk.h"

#include "TerrainGenerator.h"

Chunk::Chunk(int cx, int cz, const TerrainGenerator& generator) : m_coords(cx, cz) {
    m_blocks.fill(BlockType::AIR);

    for (int z = 0; z < CHUNK_WIDTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            int height = generator.getHeight(cx * CHUNK_WIDTH + x, cz * CHUNK_WIDTH + z);
            for (int y = 0; y <= height && y < CHUNK_HEIGHT; y++) {
                if (y == height)
                    setBlock(x, y, z, BlockType::GRASS);
                else if (y >= height - 3 && y < height)
                    setBlock(x, y, z, BlockType::DIRT);
                else if (y > 0)
                    setBlock(x, y, z, BlockType::STONE);
                else
                    setBlock(x, y, z, BlockType::BEDROCK);
            }
        }
    }
    m_state = ChunkState::Generated;
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