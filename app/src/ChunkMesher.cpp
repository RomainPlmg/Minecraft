#include "ChunkMesher.h"

#include <glad/gl.h>

#include <glm/ext/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "Chunk.h"

static opticrafter::VertexAttrib mesh_attrib[] = {
    {0, 3, GL_FLOAT, 0},                  // position
    {1, 2, GL_FLOAT, 3 * sizeof(float)},  // uvs
    {2, 1, GL_FLOAT, 5 * sizeof(float)},  // texture_id
    {3, 3, GL_FLOAT, 6 * sizeof(float)},  // normal
    {4, 1, GL_FLOAT, 9 * sizeof(float)},  // luminosity
};

static int vertexAO(bool side1, bool side2, bool corner) {
    if (side1 && side2) return 3;
    return side1 + side2 + corner;
}

void ChunkMesher::reset() { m_builder.reset(); }

MeshData ChunkMesher::build(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                            std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl, std::shared_ptr<Chunk> nfr,
                            std::shared_ptr<Chunk> nfl, std::shared_ptr<Chunk> nbr, std::shared_ptr<Chunk> nbl) {
    ZoneScopedN("ChunkMesher::build");

    assert(nf && nb && nr && nl && nfr && nfl && nbr && nbl);
    m_builder.reset();
    auto coords = chunk->coords();
    fillGrid(chunk, nf, nb, nr, nl, nfr, nfl, nbr, nbl);

    m_greedy_mesher.run(std::span(m_grid), m_builder);

    int wx = coords.x * Chunk::CHUNK_WIDTH;
    int wz = coords.y * Chunk::CHUNK_WIDTH;
    glm::ivec3 world_pos = {wx, 0, wz};

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

void ChunkMesher::fillGrid(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> nf, std::shared_ptr<Chunk> nb,
                           std::shared_ptr<Chunk> nr, std::shared_ptr<Chunk> nl, std::shared_ptr<Chunk> nfr,
                           std::shared_ptr<Chunk> nfl, std::shared_ptr<Chunk> nbr, std::shared_ptr<Chunk> nbl) {
    // Copy central chunk's data
    for (int z = 0; z < Chunk::CHUNK_WIDTH; ++z) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; ++x) {
                m_grid[index(x + 1, y, z + 1)] = chunk->getBlockNoLock(x, y, z).value_or(BlockType::AIR);
            }
        }
    }

    // Copy borders with border neighbors chunk
    for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
        for (int i = 0; i < Chunk::CHUNK_WIDTH; ++i) {
            m_grid[index(0, y, i + 1)] = nl->getBlockNoLock(Chunk::CHUNK_WIDTH - 1, y, i).value_or(BlockType::AIR);
            m_grid[index(Chunk::CHUNK_WIDTH + 1, y, i + 1)] = nr->getBlockNoLock(0, y, i).value_or(BlockType::AIR);
            m_grid[index(i + 1, y, 0)] = nb->getBlockNoLock(i, y, Chunk::CHUNK_WIDTH - 1).value_or(BlockType::AIR);
            m_grid[index(i + 1, y, Chunk::CHUNK_WIDTH + 1)] = nf->getBlockNoLock(i, y, 0).value_or(BlockType::AIR);
        }
    }

    // Copy corners with corner neighbors chunk
    for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
        m_grid[index(0, y, 0)] =
            nbl->getBlockNoLock(Chunk::CHUNK_WIDTH - 1, y, Chunk::CHUNK_WIDTH - 1).value_or(BlockType::AIR);
        m_grid[index(Chunk::CHUNK_WIDTH + 1, y, 0)] =
            nbr->getBlockNoLock(0, y, Chunk::CHUNK_WIDTH - 1).value_or(BlockType::AIR);
        m_grid[index(0, y, Chunk::CHUNK_WIDTH + 1)] =
            nfl->getBlockNoLock(Chunk::CHUNK_WIDTH - 1, y, 0).value_or(BlockType::AIR);
        m_grid[index(Chunk::CHUNK_WIDTH + 1, y, Chunk::CHUNK_WIDTH + 1)] =
            nfr->getBlockNoLock(0, y, 0).value_or(BlockType::AIR);
    }
}

std::array<int, 4> ChunkMesher::computeAO(const glm::ivec3& pos, const glm::ivec3 normal, const glm::ivec3& t,
                                          const glm::ivec3& bt) {
    ZoneScopedN("Compute AO");
    std::array<int, 4> ao;

    int i = 0;
    for (auto& uv : {glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(1, 1), glm::ivec2(-1, 1)}) {
        glm::ivec3 s1_pos = pos + normal + t * uv.x;             // side1
        glm::ivec3 s2_pos = pos + normal + bt * uv.y;            // side2
        glm::ivec3 c_pos = pos + normal + t * uv.x + bt * uv.y;  // corner

        s1_pos.y = std::clamp(s1_pos.y, 0, 255);
        s2_pos.y = std::clamp(s2_pos.y, 0, 255);
        c_pos.y = std::clamp(c_pos.y, 0, 255);

        auto side1 = getBlockLocal(s1_pos.x, s1_pos.y, s1_pos.z);
        auto side2 = getBlockLocal(s2_pos.x, s2_pos.y, s2_pos.z);
        auto corner = getBlockLocal(c_pos.x, c_pos.y, c_pos.z);

        bool bs1 = !m_registry.get(side1).transparent;
        bool bs2 = !m_registry.get(side2).transparent;
        bool bc = !m_registry.get(corner).transparent;

        ao[i] = vertexAO(bs1, bs2, bc);

        i++;
    }

    return ao;
}

inline BlockType ChunkMesher::getBlockLocal(int x, int y, int z) const {
    if (y < 0 || y >= Chunk::CHUNK_HEIGHT) return BlockType::AIR;
    return m_grid[index(x + 1, y, z + 1)];
}