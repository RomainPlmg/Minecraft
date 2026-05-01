#include "Chunk.h"

Chunk::Chunk() { m_blocks.fill(BlockType::STONE); }

BlockType Chunk::getBlock(uint8_t x, uint8_t y, uint8_t z) const { return m_blocks[index(x, y, z)]; }

void Chunk::setBlock(uint8_t x, uint8_t y, uint8_t z, BlockType type) { m_blocks[index(x, y, z)] = type; }