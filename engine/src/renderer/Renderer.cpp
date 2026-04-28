#include "opticrafter/Renderer.h"

#include <glad/gl.h>

#include "opticrafter/Logger.h"

namespace opticrafter {

Renderer::Renderer(const GLContext& ctx) {
    (void)ctx;  // Guard, need a valid context to build renderer

    m_texture_manager = std::make_unique<TextureManager>(ctx);

    setViewport({0, 0, 50, 50});
    setClearColor({30, 30, 30, 255});

    LOG_CORE_DEBUG("Renderer init successful!");
}

Renderer::~Renderer() { LOG_CORE_DEBUG("Renderer destroy successful!"); }

TextureID Renderer::createTexture(const std::string& path) { m_texture_manager->load(path); }

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

}  // namespace opticrafter