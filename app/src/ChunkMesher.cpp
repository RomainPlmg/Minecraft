#include "ChunkMesher.h"

#include <tracy/Tracy.hpp>

#include "Chunk.h"

void ChunkMesher::build(const Chunk& chunk) {
    ZoneScoped;
    std::unique_ptr<opticrafter::Mesh> mesh;
    for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (size_t y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto& block_def = m_registry.get(chunk.getBlock(x, y, z));
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Top);
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Bottom);
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Front);
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Back);
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Right);
                buildFace(chunk, {x, y, z}, block_def, MeshBuilder::Face::Left);
            }
        }
    }

    m_mesh = m_mesh_builder.build();
}

void ChunkMesher::buildFace(const Chunk& chunk, const glm::ivec3& pos, const BlockDef& block_def,
                            MeshBuilder::Face face) {
    glm::ivec3 neighbor_pos;
    opticrafter::UVRegion region;
    switch (face) {
        case MeshBuilder::Face::Top:
            neighbor_pos = {pos.x, pos.y + 1, pos.z};
            region = block_def.top;
            break;
        case MeshBuilder::Face::Bottom:
            neighbor_pos = {pos.x, pos.y - 1, pos.z};
            region = block_def.bottom;
            break;
        case MeshBuilder::Face::Front:
            neighbor_pos = {pos.x, pos.y, pos.z + 1};
            region = block_def.side;
            break;
        case MeshBuilder::Face::Back:
            neighbor_pos = {pos.x, pos.y, pos.z - 1};
            region = block_def.side;
            break;
        case MeshBuilder::Face::Right:
            neighbor_pos = {pos.x + 1, pos.y, pos.z};
            region = block_def.side;
            break;
        case MeshBuilder::Face::Left:
            neighbor_pos = {pos.x - 1, pos.y, pos.z};
            region = block_def.side;
            break;
    }

    if (chunk.contains(neighbor_pos.x, neighbor_pos.y, neighbor_pos.z)) {
        auto neighbor_type = chunk.getBlock(neighbor_pos.x, neighbor_pos.y, neighbor_pos.z);
        if (!m_registry.get(neighbor_type).transparent) return;  // If neighbor is opaque, don't generate the face
    }
    m_mesh_builder.addCubeFace({pos.x, pos.y, pos.z}, region, face);
}