#include "BlockRegistry.h"

void BlockRegistry::registerBlock(BlockType type, const BlockDef& def) {
    if (m_blocks.contains(type)) {
        LOG_WARN("Block type {} already registered.", static_cast<uint32_t>(type));
        return;
    }
    m_blocks[type] = def;
}

const BlockDef& BlockRegistry::get(BlockType type) const {
    if (!m_blocks.contains(type)) {
        LOG_ERROR("Unknown block type {}", static_cast<uint32_t>(type));
        return m_blocks.at(BlockType::UNKNOWN);
    }
    return m_blocks.at(type);
}