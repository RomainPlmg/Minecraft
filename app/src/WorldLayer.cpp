#include "WorldLayer.h"

#include <glm/glm.hpp>

WorldLayer::WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer)
    : opticrafter::Layer(stack), m_world(renderer), m_renderer(renderer) {
    m_camera = std::make_unique<opticrafter::Camera>(glm::vec3(0.f, Chunk::CHUNK_HEIGHT, 0.f));
    m_world.init();

    renderer.shaders()->loadFromFile(ASSETS_DIR "shaders/cube.vsh", ASSETS_DIR "shaders/cube.fsh");
}

void WorldLayer::onEvent(SDL_Event& event) {}

void WorldLayer::onUpdate(float dt) {
    m_world.update(dt);

    const glm::vec3 front_xz =
        glm::normalize(glm::vec3(m_camera->getFrontVector().x, 0.0f, m_camera->getFrontVector().z));
    const glm::vec3 right_xz =
        glm::normalize(glm::vec3(m_camera->getRightVector().x, 0.0f, m_camera->getRightVector().z));

    glm::vec3 move_dir(0.0f);
    if (opticrafter::Input::isKeyPressed(SDLK_W)) move_dir += front_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_S)) move_dir -= front_xz;
    if (opticrafter::Input::isKeyPressed(SDLK_A)) move_dir -= right_xz;
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
    m_world.render();
}