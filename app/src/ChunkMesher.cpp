#include "ChunkMesher.h"

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "Chunk.h"
#include "ChunkGrid.h"

void ChunkMesher::reset() { m_mesh_builder.reset(); }

ChunkRenderData ChunkMesher::build(const Chunk& chunk, const ChunkGrid& grid) {
    ZoneScopedN("ChunkMesherBuild");
    m_mesh_builder.reset();

    auto coords = chunk.coords();

    for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block_opt = chunk.getBlock(x, y, z);
                if (!block_opt || *block_opt == BlockType::AIR) continue;
                auto& block_def = m_registry.get(*block_opt);

                // Top
                auto neighbor = chunk.getBlock(x, y + 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.top, MeshBuilder::Face::Top);
                // Bottom
                neighbor = chunk.getBlock(x, y - 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_mesh_builder.addCubeFace({x, y, z}, block_def.bottom, MeshBuilder::Face::Bottom);

                // Front
                if (z == Chunk::CHUNK_WIDTH - 1) {
                    auto neighbor_chunk = grid.getChunk(coords.x, coords.y + 1);
                    if (neighbor_chunk) {
                        neighbor = neighbor_chunk->getBlock(x, y, 0);
                        if (!neighbor || m_registry.get(*neighbor).transparent) {
                            m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                        }
                    }
                } else {
                    neighbor = chunk.getBlock(x, y, z + 1);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                    }
                }

                // Back
                if (z == 0) {
                    auto neighbor_chunk = grid.getChunk(coords.x, coords.y - 1);
                    if (neighbor_chunk) {
                        neighbor = neighbor_chunk->getBlock(x, y, Chunk::CHUNK_WIDTH - 1);
                        if (!neighbor || m_registry.get(*neighbor).transparent) {
                            m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                        }
                    }
                } else {
                    neighbor = chunk.getBlock(x, y, z - 1);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                    }
                }

                // Right
                if (x == Chunk::CHUNK_WIDTH - 1) {
                    auto neighbor_chunk = grid.getChunk(coords.x + 1, coords.y);
                    if (neighbor_chunk) {
                        neighbor = neighbor_chunk->getBlock(0, y, z);
                        if (!neighbor || m_registry.get(*neighbor).transparent)
                            m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                    }
                } else {
                    neighbor = chunk.getBlock(x + 1, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                }

                // Left
                if (x == 0) {
                    auto neighbor_chunk = grid.getChunk(coords.x - 1, coords.y);
                    if (neighbor_chunk) {
                        neighbor = neighbor_chunk->getBlock(Chunk::CHUNK_WIDTH - 1, y, z);
                        if (!neighbor || m_registry.get(*neighbor).transparent)
                            m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
                    }
                } else {
                    neighbor = chunk.getBlock(x - 1, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_mesh_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
                }
            }
        }
    }

    int wx = coords.x * Chunk::CHUNK_WIDTH;
    int wz = coords.y * Chunk::CHUNK_WIDTH;
    glm::vec3 world_pos = {wx, 0, wz};

    return {
        m_mesh_builder.build(),
        opticrafter::AABB(world_pos,
                          world_pos + glm::vec3(Chunk::CHUNK_WIDTH, Chunk::CHUNK_HEIGHT, Chunk::CHUNK_WIDTH)),
        glm::translate(glm::mat4(1.f), world_pos),
    };
}