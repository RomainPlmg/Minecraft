#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <string>

#include "GLContext.h"
#include "Utils.h"

namespace opticrafter {

struct WindowSpecification {
    std::string title = "Minecraft";
    int width = 1280;
    int height = 720;
    bool vsync = true;
};

class Window {
   public:
    explicit Window(const WindowSpecification& spec = WindowSpecification());
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void swapBuffers() const;

    [[nodiscard]] SDL_Window* handle() const { return m_handle; }
    [[nodiscard]] GLContext* context() const { return m_gl_ctx.get(); }
    [[nodiscard]] Viewport viewport() const;

   private:
    WindowSpecification m_specification;
    SDL_Window* m_handle = nullptr;
    std::unique_ptr<GLContext> m_gl_ctx;
};

}  // namespace opticrafter