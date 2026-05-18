# Minecraft clone

<p align="center">
    <img src="assets/images/demo.gif" alt="Demo animation" width="800">
</p>

## Intro
A Minecraft-like voxel engine written in C++20 with OpenGL 4.6.
The goal of this project is not to provide an official version of the engine, but to show you my personal skills and how I progressed through this project.

<p align="center">
    <img src="assets/images/image.png" alt="Illustration" width="800">
</p>

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

Here on Tracy, with the `unordered_map`, we can see that build a single chunk took more than 130ms...
<p align="center">
    <img src="assets/images/tracy_build_chunk_no.png" alt="Tracy with unordered_map" width="800">
</p>

With the `array` instead `unordered_map`, I've gone down to less than 4ms:
<p align="center">
    <img src="assets/images/tracy_build_chunk_o.png" alt="Tracy with array" width="800">
</p>

### Meshing
For the meshing system, I've done for now a very simple optimization, which is the face culling. The system is very simple: during the chunk meshing, for each block I check the nerighbor cube. If this cube is opaque, I don't render the face (since it is invisible).
An example for the front face of a cube:

```cpp
std::optional<BlockType> neighbor = chunk->getBlock(x, y, z + 1); // Here I recover the cube in front of the one I am currently meshing.
if (!neighbor || m_registry.get(*neighbor).transparent) { // If the neighbor cube exists and is opaque, render the front face
    m_builder.addFrontCubeFace(block_pos, block_def.side, ao);
}
```

But there is a little difficulty. Actually this method is only working for the cubes which have their neighbors into the same chunk. But what about the cubes at chunk's boundaries ? I need to recover the neighbor cube into the neighbor chunk. Let's retrieve the front face example for a cube at chunk boundaries:
```cpp
// === Front ===
neighbor = chunk->getBlock(x, y, z + 1);
if (z == Chunk::CHUNK_WIDTH - 1) neighbor = nf->getBlock(x, y, 0); // If the cube is at the front boundary of the chunk, 
                                                                   // recover the neighbor cube into the neighbor chunk
                                                                   // (nf is for neighbor front, calculated before)
if (!neighbor || m_registry.get(*neighbor).transparent)
    m_builder.addFrontCubeFace(block_pos, block_def.side, ao);
```

Then, if a chunk is remeshed during runtime, it is mandatory to mark its neighbors "Invalidated", because if the modified cube is at boundary, the neighbor chun's mesh need to be updated.

<p align="center">
    <img src="assets/images/wireframe.png" alt="Wireframe" width="800">
</p>

### Multithreading

<p align="left">
    <img src="https://cdn-icons-png.flaticon.com/512/5578/5578703.png" alt="WIP" width="100">
</p>

## Credits & Assets
This engine uses the Faithful 32x resource pack for its visual components.
- **Credits:** All textures are property of the **Faithful Resource Pack** team.
- **Source:** https://faithfulpack.net/
- **License:** Used under the Faithful License v3. A copy of the original license is included in `./app/assets/textures/LICENSE.txt`
- **Terms:** This is a non-commercial portfolio project; no assets are monetized.
