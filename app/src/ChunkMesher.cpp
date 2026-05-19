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

static int vertexAO(bool side1, bool side2, bool corner) {
    if (side1 && side2) return 3;
    return side1 + side2 + corner;
}

void ChunkMesher::reset() { m_builder.reset(); }

MeshData ChunkMesher::build(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                            std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl) {
    ZoneScopedN("ChunkMesherBuild");

    assert(nf && nb && nr && nl);
    m_builder.reset();
    auto coords = chunk->coords();

    int wx = coords.x * Chunk::CHUNK_WIDTH;
    int wz = coords.y * Chunk::CHUNK_WIDTH;
    glm::ivec3 world_pos = {wx, 0, wz};

    for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block_opt = chunk->getBlock(x, y, z);
                if (!block_opt || *block_opt == BlockType::AIR) continue;
                auto& block_def = m_registry.get(*block_opt);
                auto block_pos = glm::ivec3(x, y, z);

                // === Top ===
                auto neighbor = chunk->getBlock(x, y + 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {0, 1, 0}, {1, 0, 0}, {0, 0, 1});
                    m_builder.addTopCubeFace(block_pos, block_def.top, ao);
                }

                // === Bottom ===
                neighbor = chunk->getBlock(x, y - 1, z);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {0, -1, 0}, {1, 0, 0}, {0, 0, 1});
                    m_builder.addBottomCubeFace(block_pos, block_def.bottom, ao);
                }

                // === Front ===
                neighbor = chunk->getBlock(x, y, z + 1);
                if (z == Chunk::CHUNK_WIDTH - 1) neighbor = nf->getBlock(x, y, 0);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {0, 0, 1}, {1, 0, 0}, {0, 1, 0});
                    m_builder.addFrontCubeFace(block_pos, block_def.side, ao);
                }

                // === Back ===
                neighbor = chunk->getBlock(x, y, z - 1);
                if (z == 0) neighbor = nb->getBlock(x, y, Chunk::CHUNK_WIDTH - 1);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {0, 0, -1}, {1, 0, 0}, {0, 1, 0});
                    m_builder.addBackCubeFace(block_pos, block_def.side, ao);
                }

                // === Right ===
                neighbor = chunk->getBlock(x + 1, y, z);
                if (x == Chunk::CHUNK_WIDTH - 1) neighbor = nr->getBlock(0, y, z);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {1, 0, 0}, {0, 0, 1}, {0, 1, 0});
                    m_builder.addRightCubeFace(block_pos, block_def.side, ao);
                }

                // === Left ===
                neighbor = chunk->getBlock(x - 1, y, z);
                if (x == 0) neighbor = nl->getBlock(Chunk::CHUNK_WIDTH - 1, y, z);
                if (!neighbor || m_registry.get(*neighbor).transparent) {
                    auto ao = computeAO(world_pos + block_pos, {-1, 0, 0}, {0, 0, 1}, {0, 1, 0});
                    m_builder.addLeftCubeFace(block_pos, block_def.side, ao);
                }
            }
        }
    }

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

std::array<int, 4> ChunkMesher::computeAO(const glm::ivec3& pos, const glm::ivec3 normal, const glm::ivec3& t,
                                          const glm::ivec3& bt) {
    std::array<int, 4> ao;

    int i = 0;
    for (auto& uv : {glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(1, 1), glm::ivec2(-1, 1)}) {
        glm::ivec3 s1_pos = pos + normal + t * uv.x;             // side1
        glm::ivec3 s2_pos = pos + normal + bt * uv.y;            // side2
        glm::ivec3 c_pos = pos + normal + t * uv.x + bt * uv.y;  // corner

        auto side1 = m_chunk_grid.getBlock(s1_pos.x, s1_pos.y, s1_pos.z);
        auto side2 = m_chunk_grid.getBlock(s2_pos.x, s2_pos.y, s2_pos.z);
        auto corner = m_chunk_grid.getBlock(c_pos.x, c_pos.y, c_pos.z);

        bool bs1 = side1.has_value() && !m_registry.get(side1.value()).transparent;
        bool bs2 = side2.has_value() && !m_registry.get(side2.value()).transparent;
        bool bc = corner.has_value() && !m_registry.get(corner.value()).transparent;

        ao[i] = vertexAO(bs1, bs2, bc);

        i++;
    }

    return ao;
}