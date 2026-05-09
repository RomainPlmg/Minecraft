#pragma once

#include <glm/glm.hpp>

#include "opticrafter/OptiCrafter.h"

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    float luminosity;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    glm::vec3 coords;
};

class MeshBuilder {
   public:
    enum class Face { Top, Bottom, Front, Back, Right, Left };

    void reset();
    void addCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, Face face);
    std::unique_ptr<opticrafter::Mesh> build();

    const MeshData& getData() const { return m_data; }

   private:
    MeshData m_data;

    void addQuad(const std::array<Vertex, 4>& vertices);
};