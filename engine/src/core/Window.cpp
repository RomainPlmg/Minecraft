#include "opticrafter/Window.h"

#include <glad/gl.h>

#include <stdexcept>
#include <tracy/TracyOpenGL.hpp>

#include "opticrafter/Logger.h"


namespace opticrafter {

Window::Window(const WindowSpecification& spec) {
    m_handle = SDL_CreateWindow(spec.title.c_str(), spec.width, spec.height,
                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL);

    if (!m_handle) {
        throw std::runtime_error(SDL_GetError());
    }

    m_gl_ctx = std::make_unique<GLContext>(*this);

    LOG_CORE_DEBUG("Window init successful!");
}

Window::~Window() {
    if (m_handle) {
        m_gl_ctx.reset();
        SDL_DestroyWindow(m_handle);
        LOG_CORE_DEBUG("Window destroy successful!");
        m_handle = nullptr;
    }
}

void Window::swapBuffers() const {
    TracyGpuCollect;
    SDL_GL_SwapWindow(m_handle);
}

void Window::setVsync(int interval) { SDL_GL_SetSwapInterval(interval); }

Viewport Window::viewport() const {
    int width, height;
    SDL_GetWindowSize(m_handle, &width, &height);
    if (width <= 0 || height <= 0) {
        return Viewport{0, 0, 0, 0};
    }

    return Viewport{0, 0, (uint32_t)width, (uint32_t)height};
}

}  // namespace opticrafter