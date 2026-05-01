#include "ChunkMesher.h"

#include "Chunk.h"

void ChunkMesher::build(const Chunk& chunk) {
    std::unique_ptr<opticrafter::Mesh> mesh;
    for (size_t y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
        for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block_type = chunk.getBlock(x, y, z);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).top, MeshBuilder::Face::Top);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).bottom, MeshBuilder::Face::Bottom);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Front);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Back);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Right);
                m_mesh_builder.addCubeFace({x, y, z}, m_registry.get(block_type).side, MeshBuilder::Face::Left);
            }
        }
    }

    m_mesh = m_mesh_builder.build();
}