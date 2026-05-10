#include "WorldLayer.h"

#include <glm/glm.hpp>
#include <tracy/Tracy.hpp>

#include "DebugLayer.h"

WorldLayer::WorldLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine)
    : opticrafter::Layer(stack, engine), m_world(engine) {
    pushOverlay<DebugLayer>();

    m_camera = std::make_unique<opticrafter::Camera>(
        glm::vec3((float)Chunk::CHUNK_WIDTH / 2.f + .5f, 10.f, (float)Chunk::CHUNK_WIDTH / 2.f + .5f));
    m_world.init();

    m_engine.renderer()->shaders()->loadFromFile(ASSETS_DIR "shaders/cube.vsh", ASSETS_DIR "shaders/cube.fsh");
}

void WorldLayer::onEvent(SDL_Event& event) {}

void WorldLayer::onUpdate(float dt) {
    ZoneScopedN("WorldLayerUpdate");
    m_world.update(dt, m_camera->position());

    const glm::vec3 front_xz = glm::normalize(glm::vec3(m_camera->frontVector().x, 0.0f, m_camera->frontVector().z));
    const glm::vec3 right_xz = glm::normalize(glm::vec3(m_camera->rightVector().x, 0.0f, m_camera->rightVector().z));

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

    m_camera->move(move_dir * 50.f * dt);

    if (opticrafter::Input::isMouseButtonPressed(SDL_BUTTON_MIDDLE)) {
        m_camera->freeze(false);
    } else
        m_camera->freeze(true);

    m_camera->update(m_engine.renderer()->viewport());
}

void WorldLayer::onRender() {
    ZoneScopedN("WorldLayerRender");
    m_engine.renderer()->beginScene(*m_camera);
    m_world.render(m_camera->frustum());
}