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
    : opticrafter::Layer(stack), m_atlas(32), m_renderer(renderer) {
    m_cube_mesh =
        std::make_unique<opticrafter::Mesh>(std::as_bytes(std::span(cube_vert)), cube_attrib, sizeof(Vertex), cube_idx);
    m_camera = std::make_unique<opticrafter::Camera>();

    // Build the texture atlas
    m_atlas.add("stone", ASSETS_DIR "textures/stone.png");
    m_atlas.add("dirt", ASSETS_DIR "textures/dirt.png");
    m_atlas.add("grass_block_side", ASSETS_DIR "textures/grass_block_side.png");
    m_atlas.add("grass_block_side_overlay", ASSETS_DIR "textures/grass_block_side_overlay.png");
    m_atlas.add("grass_block_top", ASSETS_DIR "textures/grass_block_top.png");
    m_atlas.build(renderer);

    renderer.createShaderFromFile(ASSETS_DIR "shaders/cube.vsh", ASSETS_DIR "shaders/cube.fsh");
}

void WorldLayer::onUpdate(float dt) {
    const glm::vec3 front_xz =
        glm::normalize(glm::vec3(m_camera->getFrontVector().x, 0.0f, m_camera->getFrontVector().z));
    const glm::vec3 right_xz =
        glm::normalize(glm::vec3(m_camera->getRightVector().x, 0.0f, m_camera->getRightVector().z));

    glm::vec3 move_dir(0.0f);
    if (opticrafter::Input::isKeyPressed(SDLK_Z)) move_dir += front_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_S)) move_dir -= front_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_Q)) move_dir -= right_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_D)) move_dir += right_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_SPACE)) move_dir.y += 1.f;
    if (opticrafter::Input::isKeyPressed(SDLK_LSHIFT)) move_dir.y -= 1.f;

    if (glm::length(move_dir) > 0.0f) {
        move_dir = glm::normalize(move_dir);
    }

    m_camera->move(move_dir * 5.f * dt);

    if (opticrafter::Input::isMouseButtonPressed(SDL_BUTTON_MIDDLE)) {
        m_camera->freeze(false);
    } else
        m_camera->freeze(true);

    m_camera->update();
}

void WorldLayer::onRender() {
    m_renderer.beginScene(*m_camera);
    m_renderer.draw(*m_cube_mesh, opticrafter::Material{0}, glm::mat4{1.f});
}