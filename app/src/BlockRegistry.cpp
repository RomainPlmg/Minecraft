#include "BlockRegistry.h"

void BlockRegistry::registerBlock(BlockType type, const BlockDef& def) { m_blocks[(size_t)type] = def; }

const BlockDef& BlockRegistry::get(BlockType type) const { return m_blocks.at((size_t)type); }