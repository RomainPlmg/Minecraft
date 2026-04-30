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
    void addCube(const glm::vec3& pos, const opticrafter::UVRegion& region);
    void addCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, Face face);
    void build();

    opticrafter::Mesh* mesh() const { return m_mesh.get(); }

    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<uint32_t>& getIndices() const { return m_indices; }

   private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::unique_ptr<opticrafter::Mesh> m_mesh;

    void addQuad(const std::array<Vertex, 4>& vertices);
};