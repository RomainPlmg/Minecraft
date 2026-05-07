#include "ChunkMesher.h"

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "Chunk.h"
#include "ChunkGrid.h"

ChunkRenderData ChunkMesher::build(const Chunk& chunk, const ChunkGrid& grid) {
    ZoneScoped;
    buildMeshInterior(chunk);
    buildMeshExterior(chunk, grid);

    auto coords = chunk.coords();
    glm::vec3 world_pos = {coords.x * (int)Chunk::CHUNK_WIDTH, 0, coords.y * (int)Chunk::CHUNK_WIDTH};

    return {
        m_mesh_builder.build(),
        opticrafter::AABB(world_pos,
                          world_pos + glm::vec3(Chunk::CHUNK_WIDTH, Chunk::CHUNK_HEIGHT, Chunk::CHUNK_WIDTH)),
        glm::translate(glm::mat4(1.f), world_pos),
    };
}

inline void ChunkMesher::buildMeshInterior(const Chunk& chunk) {
    ZoneScoped;
    for (size_t z = 1; z < Chunk::CHUNK_WIDTH - 1; z++) {
        for (size_t y = 1; y < Chunk::CHUNK_HEIGHT - 1; y++) {
            for (size_t x = 1; x < Chunk::CHUNK_WIDTH - 1; x++) {
                auto& block_def = m_registry.get(chunk.getBlock(x, y, z));
                // Top
                if (m_registry.get(chunk.getBlock(x, y + 1, z)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.top, MeshBuilder::Face::Top);
                // Bottom
                if (m_registry.get(chunk.getBlock(x, y - 1, z)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.bottom, MeshBuilder::Face::Bottom);
                // Front
                if (m_registry.get(chunk.getBlock(x, y, z + 1)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                // Back
                if (m_registry.get(chunk.getBlock(x, y, z - 1)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                // Right
                if (m_registry.get(chunk.getBlock(x + 1, y, z)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                // Left
                if (m_registry.get(chunk.getBlock(x - 1, y, z)).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
            }
        }
    }
}

void ChunkMesher::buildMeshExterior(const Chunk& chunk, const ChunkGrid& grid) {
    ZoneScoped;
    // For y axis, always generates the top and bottom surface of the chunk
    for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
            m_mesh_builder.addCubeFace({x, 0, z}, m_registry.get(chunk.getBlock(x, 0, z)).bottom,
                                       MeshBuilder::Face::Bottom);
            m_mesh_builder.addCubeFace({x, Chunk::CHUNK_HEIGHT - 1, z},
                                       m_registry.get(chunk.getBlock(x, Chunk::CHUNK_HEIGHT - 1, z)).bottom,
                                       MeshBuilder::Face::Top);
        }
    }
}