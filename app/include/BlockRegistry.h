#pragma once

#include "opticrafter/OptiCrafter.h"

enum class BlockType : uint8_t {
    AIR,
    GRASS,
    DIRT,
    STONE,
    COPPER_BLOCK,
    BEDROCK,
};

struct BlockDef {
    std::string name;
    opticrafter::UVRegion top;
    opticrafter::UVRegion side;
    opticrafter::UVRegion bottom;
    bool transparent;
};

class BlockRegistry {
   public:
    void registerBlock(BlockType type, const BlockDef& def);
    const BlockDef& get(BlockType type) const;

   private:
    std::array<BlockDef, 256> m_blocks;
};