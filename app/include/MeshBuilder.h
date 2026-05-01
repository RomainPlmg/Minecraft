#pragma once

#include <glm/glm.hpp>

#include "opticrafter/OptiCrafter.h"

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    float luminosity;
};

class MeshBuilder {
   public:
    enum class Face { Top, Bottom, Front, Back, Right, Left };

    void reset();
    void addCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, Face face);
    std::unique_ptr<opticrafter::Mesh> build();

    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<uint32_t>& getIndices() const { return m_indices; }

   private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    void addQuad(const std::array<Vertex, 4>& vertices);
};