#include "opticrafter/Renderer.h"

#include <glad/gl.h>

#include <tracy/Tracy.hpp>

#include "opticrafter/Camera.h"
#include "opticrafter/Logger.h"

namespace opticrafter {

Renderer::Renderer(const GLContext& ctx) {
    (void)ctx;  // Guard, need a valid context to build renderer

    m_texture_manager = std::make_unique<TextureManager>(ctx);
    m_shader_manager = std::make_unique<ShaderManager>(ctx);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    setViewport({0, 0, 1280, 720});
    setClearColor({30, 30, 30, 255});

    LOG_CORE_DEBUG("Renderer init successful!");
}

Renderer::~Renderer() { LOG_CORE_DEBUG("Renderer destroy successful!"); }

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::setClearColor(const Color& color) {
    m_clear_color = color;
    float r = (float)color.r / 255.f;
    float g = (float)color.g / 255.f;
    float b = (float)color.b / 255.f;
    float a = (float)color.a / 255.f;
    glClearColor(r, g, b, a);
}

void Renderer::setViewport(const Viewport& viewport) {
    m_viewport = viewport;
    glViewport(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h);
}

void Renderer::beginScene(const Camera& camera) {
    m_stats.draw_calls = 0;
    m_stats.triangles = 0;
    m_scene_data.proj = camera.getProjection(m_viewport);
    m_scene_data.view = camera.getView();
}

void Renderer::draw(const Mesh& mesh, const Material& material, const glm::mat4& transform) {
    ZoneScoped;
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    m_shader_manager->bind(material.id);
    m_shader_manager->setMat4(material.id, "u_proj", m_scene_data.proj);
    m_shader_manager->setMat4(material.id, "u_view", m_scene_data.view);
    m_shader_manager->setMat4(material.id, "u_model", transform);

    // Set uniforms for material
    for (auto& [name, value] : material.uniforms) {
        std::visit(
            [&](auto&& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int>)
                    m_shader_manager->setInt(material.id, name, v);
                else if constexpr (std::is_same_v<T, float>)
                    m_shader_manager->setFloat(material.id, name, v);
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    m_shader_manager->setVec2(material.id, name, v);
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    m_shader_manager->setVec3(material.id, name, v);
                else if constexpr (std::is_same_v<T, glm::mat4>)
                    m_shader_manager->setMat4(material.id, name, v);
            },
            value);
    }

    mesh.vao.bind();
    glDrawElements(GL_TRIANGLES, mesh.ebo.count(), GL_UNSIGNED_INT, nullptr);

    m_stats.draw_calls++;
    m_stats.triangles = mesh.ebo.count() / 3;
}

}  // namespace opticrafter