#include "ChunkMesher.h"

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "Chunk.h"
#include "ChunkGrid.h"

ChunkRenderData ChunkMesher::build(const Chunk& chunk, const ChunkGrid& grid) {
    ZoneScoped;
    auto coords = chunk.coords();
    int wx = coords.x * Chunk::CHUNK_WIDTH;
    int wz = coords.y * Chunk::CHUNK_WIDTH;

    for (size_t z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (size_t y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (size_t x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                const auto& block_type = chunk.getBlock(x, y, z);
                if (block_type == BlockType::AIR) continue;
                auto& block_def = m_registry.get(block_type);

                // Top
                auto neighbor = grid.getBlock({wx + x, y + 1, wz + z});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.top, MeshBuilder::Face::Top);
                // Bottom
                neighbor = grid.getBlock({wx + x, y - 1, wz + z});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.bottom, MeshBuilder::Face::Bottom);
                // Front
                neighbor = grid.getBlock({wx + x, y, wz + z + 1});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                // Back
                neighbor = grid.getBlock({wx + x, y, wz + z - 1});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                // Right
                neighbor = grid.getBlock({wx + x + 1, y, wz + z});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                // Left
                neighbor = grid.getBlock({wx + x - 1, y, wz + z});
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
            }
        }
    }

    glm::vec3 world_pos = {wx, 0, wz};

    return {
        m_mesh_builder.build(),
        opticrafter::AABB(world_pos,
                          world_pos + glm::vec3(Chunk::CHUNK_WIDTH, Chunk::CHUNK_HEIGHT, Chunk::CHUNK_WIDTH)),
        glm::translate(glm::mat4(1.f), world_pos),
    };
}