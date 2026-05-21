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

void MeshBuilder::reset() {
    m_data.indices.clear();
    m_data.vertices.clear();
}

void MeshBuilder::addFrontFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                               std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({

        Vertex{pos + glm::vec3(0 * scale.x, 0 * scale.y, 1), {0 * scale.x, 0 * scale.y}, (float)tex_id, {0, 0, 1}, .8f * s[0]},
        Vertex{pos + glm::vec3(1 * scale.x, 0 * scale.y, 1), {1 * scale.x, 0 * scale.y}, (float)tex_id, {0, 0, 1}, .8f * s[1]},
        Vertex{pos + glm::vec3(1 * scale.x, 1 * scale.y, 1), {1 * scale.x, 1 * scale.y}, (float)tex_id, {0, 0, 1}, .8f * s[2]},
        Vertex{pos + glm::vec3(0 * scale.x, 1 * scale.y, 1), {0 * scale.x, 1 * scale.y}, (float)tex_id, {0, 0, 1}, .8f * s[3]},
    });
}

void MeshBuilder::addBackFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                              std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0 * scale.x, 0 * scale.y, 0), {0 * scale.x, 0 * scale.y}, (float)tex_id, {0, 0, -1}, .8f * s[0]},
        Vertex{pos + glm::vec3(0 * scale.x, 1 * scale.y, 0), {0 * scale.x, 1 * scale.y}, (float)tex_id, {0, 0, -1}, .8f * s[3]},
        Vertex{pos + glm::vec3(1 * scale.x, 1 * scale.y, 0), {1 * scale.x, 1 * scale.y}, (float)tex_id, {0, 0, -1}, .8f * s[2]},
        Vertex{pos + glm::vec3(1 * scale.x, 0 * scale.y, 0), {1 * scale.x, 0 * scale.y}, (float)tex_id, {0, 0, -1}, .8f * s[1]},
    });
}

void MeshBuilder::addRightFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                               std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(1, 0 * scale.y, 0 * scale.x), {0 * scale.x, 0 * scale.y}, (float)tex_id, {1, 0, 0}, .8f * s[0]},
        Vertex{pos + glm::vec3(1, 1 * scale.y, 0 * scale.x), {0 * scale.x, 1 * scale.y}, (float)tex_id, {1, 0, 0}, .8f * s[3]},
        Vertex{pos + glm::vec3(1, 1 * scale.y, 1 * scale.x), {1 * scale.x, 1 * scale.y}, (float)tex_id, {1, 0, 0}, .8f * s[2]},
        Vertex{pos + glm::vec3(1, 0 * scale.y, 1 * scale.x), {1 * scale.x, 0 * scale.y}, (float)tex_id, {1, 0, 0}, .8f * s[1]},
    });
}

void MeshBuilder::addLeftFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                              std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0, 0 * scale.y, 0 * scale.x), {0 * scale.x, 0 * scale.y}, (float)tex_id, {-1, 0, 0}, .8f * s[0]},
        Vertex{pos + glm::vec3(0, 0 * scale.y, 1 * scale.x), {1 * scale.x, 0 * scale.y}, (float)tex_id, {-1, 0, 0}, .8f * s[1]},
        Vertex{pos + glm::vec3(0, 1 * scale.y, 1 * scale.x), {1 * scale.x, 1 * scale.y}, (float)tex_id, {-1, 0, 0}, .8f * s[2]},
        Vertex{pos + glm::vec3(0, 1 * scale.y, 0 * scale.x), {0 * scale.x, 1 * scale.y}, (float)tex_id, {-1, 0, 0}, .8f * s[3]},
    });
}

void MeshBuilder::addTopFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                             std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0 * scale.x, 1, 0 * scale.y), {0 * scale.x, 0 * scale.y}, (float)tex_id, {0, 1, 0}, 1.f * s[0]},
        Vertex{pos + glm::vec3(0 * scale.x, 1, 1 * scale.y), {0 * scale.x, 1 * scale.y}, (float)tex_id, {0, 1, 0}, 1.f * s[3]},
        Vertex{pos + glm::vec3(1 * scale.x, 1, 1 * scale.y), {1 * scale.x, 1 * scale.y}, (float)tex_id, {0, 1, 0}, 1.f * s[2]},
        Vertex{pos + glm::vec3(1 * scale.x, 1, 0 * scale.y), {1 * scale.x, 0 * scale.y}, (float)tex_id, {0, 1, 0}, 1.f * s[1]},
    });
}

void MeshBuilder::addBottomFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                                std::array<int, 4>& ao) {
    std::array<float, 4> s = {getAOFactor(ao[0]), getAOFactor(ao[1]), getAOFactor(ao[2]), getAOFactor(ao[3])};
    addQuad({
        Vertex{pos + glm::vec3(0 * scale.x, 0, 0 * scale.y), {0 * scale.x, 0 * scale.y}, (float)tex_id, {0, -1, 0}, .6f * s[0]},
        Vertex{pos + glm::vec3(1 * scale.x, 0, 0 * scale.y), {1 * scale.x, 0 * scale.y}, (float)tex_id, {0, -1, 0}, .6f * s[3]},
        Vertex{pos + glm::vec3(1 * scale.x, 0, 1 * scale.y), {1 * scale.x, 1 * scale.y}, (float)tex_id, {0, -1, 0}, .6f * s[2]},
        Vertex{pos + glm::vec3(0 * scale.x, 0, 1 * scale.y), {0 * scale.x, 1 * scale.y}, (float)tex_id, {0, -1, 0}, .6f * s[1]},
    });
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
