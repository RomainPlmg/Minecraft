# Minecraft clone

## Intro
Hi guys! I present to you my own Minecraft engine, wrote in C++ with OpenGL 4.6.
The goal of this project is not to provide an official version of the engine, but to show you my personal skills and how I progressed through this project :)

## Requirements
This project requires the following requirements:
- A compiler compatible with C++20
- An OS and GPU which support OpenGL 4.6 (Linux or Windows, MacOS doesn't supports OpenGL since version 4.1)
- CMake > v3.30.0

## Technical description
In this part I will try to explain the different technical choices I made and why.

### Memory consumption
First of all, before any rendering, I worked on the memory consumption. Minecraft is a set of chunks, which contains 16\*16\*256 blocks.
With a render distance of 12 for examples, I need to store more than 37 millions of blocks. You can't store all block properties in each of them, because you will just burn your RAM! We must therefore store our blocks intelligently.  
The solution I've chosen is to make a very simple type registry system. Instead of storing all block properties in each block, I just store the block type (grass, stone, dirt, etc). This can be a simple enumeration based on a uint8:

```c++
enum class BlockType : uint8_t {
    AIR,
    GRASS,
    DIRT,
    STONE,
    COPPER_BLOCK,
    BEDROCK,
};
```

If I want to access block properties (transparent, UVs, affected by gravity, etc.), I create a registry where, for each type of block, I can recover at any moment their properties:

```c++
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
```

In the block registry, I've chosen to store my blocks into an array instead of a `std::unordered_map<BlockType, BlockDef>`. This is a choice of memory continuity. `unordered_map` strutures dispatch all the data in the memory. This greatly reduces CPU cache usage, and therefore performances. `std::array`is continuous in memory, which optimizes the CPU's cache usage.

### Meshing

### Multithreading

## Credits & Assets
This engine uses the Faithful 32x resource pack for its visual components.
- **Credits:** All textures are property of the **Faithful Resource Pack** team.
- **Source:** https://faithfulpack.net/
- **License:** Used under the Faithful License v3. A copy of the original license is included in `./app/assets/textures/LICENSE.txt`
- **Terms:** This is a non-commercial portfolio project; no assets are monetized.
