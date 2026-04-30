#include "WorldLayer.h"

#include <glm/glm.hpp>

WorldLayer::WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer)
    : opticrafter::Layer(stack), m_atlas(32), m_renderer(renderer) {
    m_camera = std::make_unique<opticrafter::Camera>(glm::vec3(1.f, 0.f, 8.f));

    // Build the texture atlas
    m_atlas.add("stone", ASSETS_DIR "textures/stone.png");
    m_atlas.add("dirt", ASSETS_DIR "textures/dirt.png");
    m_atlas.add("grass_block_side", ASSETS_DIR "textures/grass_block_side.png");
    m_atlas.add("grass_block_side_overlay", ASSETS_DIR "textures/grass_block_side_overlay.png");
    m_atlas.add("grass_block_top", ASSETS_DIR "textures/grass_block_top.png");
    m_atlas.build(renderer);

    m_mesh_builder.addCube({0, 0, 0}, m_atlas.region("stone"));
    m_mesh_builder.addCube({2, 0, 0}, m_atlas.region("dirt"));
    m_mesh_builder.build();

    renderer.shaders()->loadFromFile(ASSETS_DIR "shaders/cube.vsh", ASSETS_DIR "shaders/cube.fsh");
}

void WorldLayer::onUpdate(float dt) {
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
    m_renderer.textures()->bind(m_atlas.handle());
    m_renderer.draw(*m_mesh_builder.mesh(), opticrafter::Material{0}, glm::mat4{1.f});
}