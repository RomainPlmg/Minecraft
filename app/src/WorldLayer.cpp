#include "WorldLayer.h"

#include <glad/gl.h>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
};

static Vertex cube_vert[] = {
    {{-1.f, -1.f, -1.0f}},  // bottom-left-front
    {{1.f, -1.f, -1.0f}},   // bottom-right-front
    {{1.f, 1.f, -1.0f}},    // top-right-front
    {{-1.f, 1.f, -1.0f}},   // top-left-front
    {{-1.f, -1.f, 1.0f}},   // bottom-left-back
    {{1.f, -1.f, 1.0f}},    // bottom-right-back
    {{1.f, 1.f, 1.0f}},     // top-right-back
    {{-1.f, 1.f, 1.0f}},    // top-left-back
};

static uint32_t cube_idx[] = {
    // Front
    0,
    1,
    3,
    1,
    2,
    3,
    // Right
    1,
    5,
    2,
    5,
    6,
    2,
    // Back
    5,
    4,
    6,
    4,
    7,
    6,
    // Left
    4,
    0,
    7,
    0,
    3,
    7,
    // Bottom
    4,
    5,
    0,
    5,
    1,
    0,
    // Top
    3,
    2,
    7,
    2,
    6,
    7,
};

static opticrafter::VertexAttrib cube_attrib[] = {
    {0, 3, GL_FLOAT, 0},
};

WorldLayer::WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer)
    : opticrafter::Layer(stack), m_renderer(renderer) {
    m_cube_mesh =
        std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(cube_vert)), cube_attrib, sizeof(Vertex), cube_idx);
    m_renderer.createShaderFromFile(ASSETS_DIR "shaders/cube.vsh", ASSETS_DIR "shaders/cube.fsh");
}

void WorldLayer::onUpdate(float dt) {}

void WorldLayer::onRender() { m_renderer.draw(*m_cube_mesh, 0); }