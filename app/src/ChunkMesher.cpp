#include "ChunkMesher.h"

#include "Chunk.h"

void ChunkMesher::build(const Chunk& chunk) {
    std::unique_ptr<opticrafter::Mesh> mesh;
    for (size_t y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
        for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                // auto block_type = chunk.getBlock(x, y, z);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).top, MeshBuilder::Face::Top);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).bottom, MeshBuilder::Face::Bottom);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Front);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Back);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Right);
                // m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Left);

                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Top);
                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Bottom);
                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Front);
                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Back);
                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Right);
                buildFace(chunk, {x, y, z}, MeshBuilder::Face::Left);
            }
        }
    }

    m_mesh = m_mesh_builder.build();
}

void ChunkMesher::buildFace(const Chunk& chunk, const glm::vec3 pos, MeshBuilder::Face face) {
    auto block_type = chunk.getBlock(pos.x, pos.y, pos.z);
    glm::vec3 neighbor_pos;
    opticrafter::UVRegion region;
    switch (face) {
        case MeshBuilder::Face::Top:
            neighbor_pos = {pos.x, pos.y + 1, pos.z};
            region = m_registry.get(block_type).top;
            break;
        case MeshBuilder::Face::Bottom:
            neighbor_pos = {pos.x, pos.y - 1, pos.z};
            region = m_registry.get(block_type).bottom;
            break;
        case MeshBuilder::Face::Front:
            neighbor_pos = {pos.x, pos.y, pos.z + 1};
            region = m_registry.get(block_type).side;
            break;
        case MeshBuilder::Face::Back:
            neighbor_pos = {pos.x, pos.y, pos.z - 1};
            region = m_registry.get(block_type).side;
            break;
        case MeshBuilder::Face::Right:
            neighbor_pos = {pos.x + 1, pos.y, pos.z};
            region = m_registry.get(block_type).side;
            break;
        case MeshBuilder::Face::Left:
            neighbor_pos = {pos.x - 1, pos.y, pos.z};
            region = m_registry.get(block_type).side;
            break;
    }

    if (chunk.contains(neighbor_pos.x, neighbor_pos.y, neighbor_pos.z)) {
        auto neighbor_type = chunk.getBlock(neighbor_pos.x, neighbor_pos.y, neighbor_pos.z);
        if (!m_registry.get(neighbor_type).transparent) return;  // If neighbor is opaque, don't generate the face
    }
    m_mesh_builder.addCubeFace({pos.x, pos.y, pos.z}, region, face);
}