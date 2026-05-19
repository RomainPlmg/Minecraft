#include "MeshBuilder.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>

// Helper function to recover light level from ambient occlusion factor
static float getAOFactor(int ao) {
    switch (ao) {
        case 0:
            return 1.0f;
        case 1:
            return 0.8f;
        case 2:
            return 0.6f;
        case 3:
            return 0.4f;
        default:
            return 1.0f;
    }
}

static opticrafter::VertexAttrib mesh_attrib[] = {
    {0, 3, GL_FLOAT, 0},
    {1, 2, GL_FLOAT, 3 * sizeof(float)},
    {2, 3, GL_FLOAT, 5 * sizeof(float)},
    {3, 1, GL_FLOAT, 8 * sizeof(float)},
};

void MeshBuilder::reset() {
    m_data.indices.clear();
    m_data.vertices.clear();
}

void MeshBuilder::addFrontCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({

        Vertex{pos + glm::vec3(0, 0, 1), {region.uv_min.x, region.uv_min.y}, {0, 0, 1}, .8f * s[0]},
        Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_min.y}, {0, 0, 1}, .8f * s[1]},
        Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {0, 0, 1}, .8f * s[2]},
        Vertex{pos + glm::vec3(0, 1, 1), {region.uv_min.x, region.uv_max.y}, {0, 0, 1}, .8f * s[3]},
    });
}

void MeshBuilder::addBackCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {0, 0, -1}, .8f * s[0]},
        Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_max.y}, {0, 0, -1}, .8f * s[3]},
        Vertex{pos + glm::vec3(1, 1, 0), {region.uv_max.x, region.uv_max.y}, {0, 0, -1}, .8f * s[2]},
        Vertex{pos + glm::vec3(1, 0, 0), {region.uv_max.x, region.uv_min.y}, {0, 0, -1}, .8f * s[1]},
    });
}

void MeshBuilder::addRightCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(1, 0, 0), {region.uv_min.x, region.uv_min.y}, {1, 0, 0}, .8f * s[0]},
        Vertex{pos + glm::vec3(1, 1, 0), {region.uv_min.x, region.uv_max.y}, {1, 0, 0}, .8f * s[3]},
        Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {1, 0, 0}, .8f * s[2]},
        Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_min.y}, {1, 0, 0}, .8f * s[1]},
    });
}

void MeshBuilder::addLeftCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {-1, 0, 0}, .8f * s[0]},
        Vertex{pos + glm::vec3(0, 0, 1), {region.uv_max.x, region.uv_min.y}, {-1, 0, 0}, .8f * s[1]},
        Vertex{pos + glm::vec3(0, 1, 1), {region.uv_max.x, region.uv_max.y}, {-1, 0, 0}, .8f * s[2]},
        Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_max.y}, {-1, 0, 0}, .8f * s[3]},
    });
}

void MeshBuilder::addTopCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0, 1, 0), {region.uv_min.x, region.uv_min.y}, {0, 1, 0}, 1.f * s[0]},
        Vertex{pos + glm::vec3(0, 1, 1), {region.uv_min.x, region.uv_max.y}, {0, 1, 0}, 1.f * s[3]},
        Vertex{pos + glm::vec3(1, 1, 1), {region.uv_max.x, region.uv_max.y}, {0, 1, 0}, 1.f * s[2]},
        Vertex{pos + glm::vec3(1, 1, 0), {region.uv_max.x, region.uv_min.y}, {0, 1, 0}, 1.f * s[1]},
    });
}

void MeshBuilder::addBottomCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0, 0, 0), {region.uv_min.x, region.uv_min.y}, {0, -1, 0}, .6f * s[0]},
        Vertex{pos + glm::vec3(1, 0, 0), {region.uv_max.x, region.uv_min.y}, {0, -1, 0}, .6f * s[3]},
        Vertex{pos + glm::vec3(1, 0, 1), {region.uv_max.x, region.uv_max.y}, {0, -1, 0}, .6f * s[2]},
        Vertex{pos + glm::vec3(0, 0, 1), {region.uv_min.x, region.uv_max.y}, {0, -1, 0}, .6f * s[1]},
    });
}

std::unique_ptr<opticrafter::Mesh> MeshBuilder::build() {
    return std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(m_data.vertices)), mesh_attrib, sizeof(Vertex),
                                               m_data.indices);
}

void MeshBuilder::addQuad(const std::array<Vertex, 4>& vertices) {
    uint32_t base_index = static_cast<uint32_t>(m_data.vertices.size());

    for (const auto& vertex : vertices) {
        m_data.vertices.push_back(vertex);
    }

    // Two triangles
    m_data.indices.push_back(base_index + 0);
    m_data.indices.push_back(base_index + 1);
    m_data.indices.push_back(base_index + 2);

    m_data.indices.push_back(base_index + 2);
    m_data.indices.push_back(base_index + 3);
    m_data.indices.push_back(base_index + 0);
}
