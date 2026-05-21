#pragma once

#include <glm/glm.hpp>

#include "opticrafter/OptiCrafter.h"

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    float texture_id;
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

    void addFrontFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                      std::array<int, 4>& ao);
    void addBackFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                     std::array<int, 4>& ao);
    void addRightFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                      std::array<int, 4>& ao);
    void addLeftFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                     std::array<int, 4>& ao);
    void addTopFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id, std::array<int, 4>& ao);
    void addBottomFace(const glm::vec3& pos, const glm::vec2 scale, opticrafter::TextureID tex_id,
                       std::array<int, 4>& ao);

    const MeshData& getData() const { return m_data; }

   private:
    MeshData m_data;

    void addQuad(const std::array<Vertex, 4>& vertices);
};