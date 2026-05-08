#include "MeshBuilder.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>

static opticrafter::VertexAttrib mesh_attrib[] = {
    {0, 3, GL_FLOAT, 0},
    {1, 2, GL_FLOAT, 3 * sizeof(float)},
    {2, 3, GL_FLOAT, 5 * sizeof(float)},
    {3, 1, GL_FLOAT, 8 * sizeof(float)},
};

void MeshBuilder::reset() {
    m_indices.clear();
    m_vertices.clear();
}

void MeshBuilder::addCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, Face face) {
    switch (face) {
        case Face::Top:
            addQuad({
                Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_min.y}, {0, 1, 0}, 1.f},
                Vertex{pos + glm::vec3(0, 1, 1), {region.uv_min.x, region.uv_max.y}, {0, 1, 0}, 1.f},
                Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {0, 1, 0}, 1.f},
                Vertex{pos + glm::vec3(1, 1, 0), {region.uv_max.x, region.uv_min.y}, {0, 1, 0}, 1.f},
            });
            break;
        case Face::Bottom:
            addQuad({
                Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {0, -1, 0}, .6f},
                Vertex{pos + glm::vec3(1, 0, 0), {region.uv_max.x, region.uv_min.y}, {0, -1, 0}, .6f},
                Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_max.y}, {0, -1, 0}, .6f},
                Vertex{pos + glm::vec3(0, 0, 1), {region.uv_min.x, region.uv_max.y}, {0, -1, 0}, .6f},
            });
            break;
        case Face::Front:
            addQuad({
                Vertex{pos + glm::vec3(0, 0, 1), {region.uv_min.x, region.uv_min.y}, {0, 0, 1}, .8f},
                Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_min.y}, {0, 0, 1}, .8f},
                Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {0, 0, 1}, .8f},
                Vertex{pos + glm::vec3(0, 1, 1), {region.uv_min.x, region.uv_max.y}, {0, 0, 1}, .8f},
            });
            break;
        case Face::Back:
            addQuad({
                Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {0, 0, -1}, .8f},
                Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_max.y}, {0, 0, -1}, .8f},
                Vertex{pos + glm::vec3(1, 1, 0), {region.uv_max.x, region.uv_max.y}, {0, 0, -1}, .8f},
                Vertex{pos + glm::vec3(1, 0, 0), {region.uv_max.x, region.uv_min.y}, {0, 0, -1}, .8f},
            });
            break;
        case Face::Right:
            addQuad({
                Vertex{pos + glm::vec3(1, 0, 0), {region.uv_min.x, region.uv_min.y}, {1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(1, 1, 0), {region.uv_min.x, region.uv_max.y}, {1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_min.y}, {1, 0, 0}, .8f},
            });
            break;
        case Face::Left:
            addQuad({
                Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {-1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(0, 0, 1), {region.uv_max.x, region.uv_min.y}, {-1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(0, 1, 1), {region.uv_max.x, region.uv_max.y}, {-1, 0, 0}, .8f},
                Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_max.y}, {-1, 0, 0}, .8f},
            });
            break;
    }
}

std::unique_ptr<opticrafter::Mesh> MeshBuilder::build() {
    return std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(m_vertices)), mesh_attrib, sizeof(Vertex),
                                               m_indices);
}

void MeshBuilder::addQuad(const std::array<Vertex, 4>& vertices) {
    uint32_t base_index = static_cast<uint32_t>(m_vertices.size());

    for (const auto& vertex : vertices) {
        m_vertices.push_back(vertex);
    }

    // Two triangles
    m_indices.push_back(base_index + 0);
    m_indices.push_back(base_index + 1);
    m_indices.push_back(base_index + 2);

    m_indices.push_back(base_index + 2);
    m_indices.push_back(base_index + 3);
    m_indices.push_back(base_index + 0);
}
