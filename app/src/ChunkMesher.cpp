#include "ChunkMesher.h"

#include <glad/gl.h>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "Chunk.h"

static opticrafter::VertexAttrib mesh_attrib[] = {
    {0, 3, GL_FLOAT, 0},
    {1, 2, GL_FLOAT, 3 * sizeof(float)},
    {2, 3, GL_FLOAT, 5 * sizeof(float)},
    {3, 1, GL_FLOAT, 8 * sizeof(float)},
};

void ChunkMesher::reset() { m_builder.reset(); }

MeshData ChunkMesher::build(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                            std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl) {
    ZoneScopedN("ChunkMesherBuild");

    assert(nf && nb && nr && nl);

    m_builder.reset();

    auto coords = chunk->coords();

    for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block_opt = chunk->getBlock(x, y, z);
                if (!block_opt || *block_opt == BlockType::AIR) continue;
                auto& block_def = m_registry.get(*block_opt);

                // Top
                auto neighbor = chunk->getBlock(x, y + 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_builder.addCubeFace({x, y, z}, block_def.top, MeshBuilder::Face::Top);
                // Bottom
                neighbor = chunk->getBlock(x, y - 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent)
                    m_builder.addCubeFace({x, y, z}, block_def.bottom, MeshBuilder::Face::Bottom);

                // Front
                if (z == Chunk::CHUNK_WIDTH - 1) {
                    neighbor = nf->getBlock(x, y, 0);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                    }

                } else {
                    neighbor = chunk->getBlock(x, y, z + 1);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Front);
                    }
                }

                // Back
                if (z == 0) {
                    neighbor = nb->getBlock(x, y, Chunk::CHUNK_WIDTH - 1);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                    }

                } else {
                    neighbor = chunk->getBlock(x, y, z - 1);
                    if (!neighbor || m_registry.get(*neighbor).transparent) {
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Back);
                    }
                }

                // Right
                if (x == Chunk::CHUNK_WIDTH - 1) {
                    neighbor = nr->getBlock(0, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                } else {
                    neighbor = chunk->getBlock(x + 1, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Right);
                }

                // Left
                if (x == 0) {
                    neighbor = nl->getBlock(Chunk::CHUNK_WIDTH - 1, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
                } else {
                    neighbor = chunk->getBlock(x - 1, y, z);
                    if (!neighbor || m_registry.get(*neighbor).transparent)
                        m_builder.addCubeFace({x, y, z}, block_def.side, MeshBuilder::Face::Left);
                }
            }
        }
    }

    int wx = coords.x * Chunk::CHUNK_WIDTH;
    int wz = coords.y * Chunk::CHUNK_WIDTH;
    glm::vec3 world_pos = {wx, 0, wz};

    return {
        .vertices = m_builder.getData().vertices,
        .indices = m_builder.getData().indices,
        .coords = {coords.x, 0, coords.y},
    };
}

ChunkRenderData ChunkMesher::uploadToGPU(MeshData&& data) {
    int wx = data.coords.x * Chunk::CHUNK_WIDTH;
    int wz = data.coords.z * Chunk::CHUNK_WIDTH;
    glm::vec3 world_pos = {wx, 0, wz};

    auto mesh = std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(data.vertices)), mesh_attrib,
                                                    sizeof(Vertex), data.indices);

    return {
        std::move(mesh),
        opticrafter::AABB(world_pos,
                          world_pos + glm::vec3(Chunk::CHUNK_WIDTH, Chunk::CHUNK_HEIGHT, Chunk::CHUNK_WIDTH)),
        glm::translate(glm::mat4(1.f), world_pos),
    };
}