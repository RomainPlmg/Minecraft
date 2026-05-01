#pragma once

#include "opticrafter/OptiCrafter.h"

enum class BlockType : uint8_t {
    UNKNOWN,
    GRASS,
    DIRT,
    STONE,
};

struct BlockDef {
    std::string name;
    opticrafter::UVRegion top;
    opticrafter::UVRegion side;
    opticrafter::UVRegion bottom;
};

class BlockRegistry {
   public:
    void registerBlock(BlockType type, const BlockDef& def);
    const BlockDef& get(BlockType type) const;

   private:
    std::unordered_map<BlockType, BlockDef> m_blocks;
};