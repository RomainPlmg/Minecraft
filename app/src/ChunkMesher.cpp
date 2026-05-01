#include "ChunkMesher.h"

#include "Chunk.h"

void ChunkMesher::build(const Chunk& chunk) {
    std::unique_ptr<opticrafter::Mesh> mesh;
    for (size_t y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
        for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                for (const auto& face : {MeshBuilder::Face::Top, MeshBuilder::Face::Bottom, MeshBuilder::Face::Front,
                                         MeshBuilder::Face::Back, MeshBuilder::Face::Right, MeshBuilder::Face::Left}) {
                    auto block_type = chunk.getBlock(x, y, z);
                    m_mesh_builder.addCubeFace({x, y, z}, m_atlas.region(m_registry.get(block_type).name), face);
                }
            }
        }
    }

    m_mesh = m_mesh_builder.build();
}