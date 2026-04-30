#include "MeshBuilder.h"

#include <glad/gl.h>

static opticrafter::VertexAttrib mesh_attrib[] = {
    {0, 3, GL_FLOAT, 0},
    {1, 2, GL_FLOAT, 3 * sizeof(float)},
    {2, 3, GL_FLOAT, 7 * sizeof(float)},
    {3, 1, GL_FLOAT, 10 * sizeof(float)},
};

void MeshBuilder::reset() {
    m_vertices.clear();
    m_indices.clear();
}

void MeshBuilder::addCube(const glm::vec3& position) {
    // Top face (+Y)
    addQuad({
        Vertex{position + glm::vec3(0, 1, 0), {0.f, 0.f}, {0, 1, 0}, 1.f},
        Vertex{position + glm::vec3(0, 1, 1), {0.f, 1.f}, {0, 1, 0}, 1.f},
        Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {0, 1, 0}, 1.f},
        Vertex{position + glm::vec3(1, 1, 0), {1.f, 0.f}, {0, 1, 0}, 1.f},
    });

    // Bottom face (-Y)
    addQuad({
        Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {0, -1, 0}, .6f},
        Vertex{position + glm::vec3(1, 0, 0), {1.f, 0.f}, {0, -1, 0}, .6f},
        Vertex{position + glm::vec3(1, 0, 1), {1.f, 1.f}, {0, -1, 0}, .6f},
        Vertex{position + glm::vec3(0, 0, 1), {0.f, 1.f}, {0, -1, 0}, .6f},
    });

    // Front face (+Z)
    addQuad({
        Vertex{position + glm::vec3(0, 0, 1), {0.f, 0.f}, {0, 0, 1}, .8f},
        Vertex{position + glm::vec3(1, 0, 1), {1.f, 0.f}, {0, 0, 1}, .8f},
        Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {0, 0, 1}, .8f},
        Vertex{position + glm::vec3(0, 1, 1), {0.f, 1.f}, {0, 0, 1}, .8f},
    });

    // Back face (-Z)
    addQuad({
        Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {0, 0, -1}, .8f},
        Vertex{position + glm::vec3(0, 1, 0), {0.f, 1.f}, {0, 0, -1}, .8f},
        Vertex{position + glm::vec3(1, 1, 0), {1.f, 1.f}, {0, 0, -1}, .8f},
        Vertex{position + glm::vec3(1, 0, 0), {1.f, 0.f}, {0, 0, -1}, .8f},
    });

    // Right face (+X)
    addQuad({
        Vertex{position + glm::vec3(1, 0, 0), {0.f, 0.f}, {1, 0, 0}, .8f},
        Vertex{position + glm::vec3(1, 1, 0), {0.f, 1.f}, {1, 0, 0}, .8f},
        Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {1, 0, 0}, .8f},
        Vertex{position + glm::vec3(1, 0, 1), {1.f, 0.f}, {1, 0, 0}, .8f},
    });

    // Left face (-X)
    addQuad({
        Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {-1, 0, 0}, .8f},
        Vertex{position + glm::vec3(0, 0, 1), {1.f, 0.f}, {-1, 0, 0}, .8f},
        Vertex{position + glm::vec3(0, 1, 1), {1.f, 1.f}, {-1, 0, 0}, .8f},
        Vertex{position + glm::vec3(0, 1, 0), {0.f, 1.f}, {-1, 0, 0}, .8f},
    });
}

void MeshBuilder::addCubeFace(const glm::vec3& position, Face face) {
    switch (face) {
        case Face::Top:
            addQuad({
                Vertex{position + glm::vec3(0, 1, 0), {0.f, 0.f}, {0, 1, 0}, 1.f},
                Vertex{position + glm::vec3(0, 1, 1), {0.f, 1.f}, {0, 1, 0}, 1.f},
                Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {0, 1, 0}, 1.f},
                Vertex{position + glm::vec3(1, 1, 0), {1.f, 0.f}, {0, 1, 0}, 1.f},
            });
            break;
        case Face::Bottom:
            addQuad({
                Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {0, -1, 0}, .6f},
                Vertex{position + glm::vec3(1, 0, 0), {1.f, 0.f}, {0, -1, 0}, .6f},
                Vertex{position + glm::vec3(1, 0, 1), {1.f, 1.f}, {0, -1, 0}, .6f},
                Vertex{position + glm::vec3(0, 0, 1), {0.f, 1.f}, {0, -1, 0}, .6f},
            });
            break;
        case Face::Front:
            addQuad({
                Vertex{position + glm::vec3(0, 0, 1), {0.f, 0.f}, {0, 0, 1}, .8f},
                Vertex{position + glm::vec3(1, 0, 1), {1.f, 0.f}, {0, 0, 1}, .8f},
                Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {0, 0, 1}, .8f},
                Vertex{position + glm::vec3(0, 1, 1), {0.f, 1.f}, {0, 0, 1}, .8f},
            });
            break;
        case Face::Back:
            addQuad({
                Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {0, 0, -1}, .8f},
                Vertex{position + glm::vec3(0, 1, 0), {0.f, 1.f}, {0, 0, -1}, .8f},
                Vertex{position + glm::vec3(1, 1, 0), {1.f, 1.f}, {0, 0, -1}, .8f},
                Vertex{position + glm::vec3(1, 0, 0), {1.f, 0.f}, {0, 0, -1}, .8f},
            });
            break;
        case Face::Right:
            addQuad({
                Vertex{position + glm::vec3(1, 0, 0), {0.f, 0.f}, {1, 0, 0}, .8f},
                Vertex{position + glm::vec3(1, 1, 0), {0.f, 1.f}, {1, 0, 0}, .8f},
                Vertex{position + glm::vec3(1, 1, 1), {1.f, 1.f}, {1, 0, 0}, .8f},
                Vertex{position + glm::vec3(1, 0, 1), {1.f, 0.f}, {1, 0, 0}, .8f},
            });
            break;
        case Face::Left:
            addQuad({
                Vertex{position + glm::vec3(0, 0, 0), {0.f, 0.f}, {-1, 0, 0}, .8f},
                Vertex{position + glm::vec3(0, 0, 1), {1.f, 0.f}, {-1, 0, 0}, .8f},
                Vertex{position + glm::vec3(0, 1, 1), {1.f, 1.f}, {-1, 0, 0}, .8f},
                Vertex{position + glm::vec3(0, 1, 0), {0.f, 1.f}, {-1, 0, 0}, .8f},
            });
            break;
    }
}

void MeshBuilder::build() {
    m_mesh.reset();
    m_mesh = std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(m_vertices)), mesh_attrib, sizeof(Vertex),
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
