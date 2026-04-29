#include "opticrafter/GLContext.h"

#include <glad/gl.h>

#include <stdexcept>

#include "opticrafter/Logger.h"
#include "opticrafter/Window.h"

namespace opticrafter {

GLContext::GLContext(const Window& window) {
    m_handle = SDL_GL_CreateContext(window.handle());

    if (!m_handle) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!SDL_GL_MakeCurrent(window.handle(), m_handle)) {
        throw std::runtime_error(SDL_GetError());
    }

    int version = gladLoadGL(SDL_GL_GetProcAddress);
    if (!version) {
        throw std::runtime_error("Fail to load GLAD.");
    }
    LOG_CORE_INFO("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    LOG_CORE_DEBUG("GLContext init successful!");
}

GLContext::~GLContext() {
    if (m_handle) {
        if (!SDL_GL_DestroyContext(m_handle)) {
            LOG_CORE_ERROR("GLContext destroy failed: {}", SDL_GetError());
        } else {
            LOG_CORE_DEBUG("GLContext destroy successful!");
        }
        m_handle = nullptr;
    }
}

}  // namespace opticrafter