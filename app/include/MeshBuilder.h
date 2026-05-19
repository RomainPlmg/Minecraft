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
    MeshBuilder() = default;
    MeshBuilder(const MeshBuilder&) = delete;
    MeshBuilder& operator=(const MeshBuilder&) = delete;

    enum class Face { Top, Bottom, Front, Back, Right, Left };

    void reset();
    void addCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<float, 4>& ao, Face face);
    
    void addFrontCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);
    void addBackCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);
    void addRightCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);
    void addLeftCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);
    void addTopCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);
    void addBottomCubeFace(const glm::vec3& pos, const opticrafter::UVRegion& region, std::array<int, 4>& ao);

    std::unique_ptr<opticrafter::Mesh> build();

    const MeshData& getData() const { return m_data; }

   private:
    MeshData m_data;

    void addQuad(const std::array<Vertex, 4>& vertices);
};