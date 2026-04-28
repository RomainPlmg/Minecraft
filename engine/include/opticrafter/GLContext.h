#pragma once

#include <SDL3/SDL.h>

namespace opticrafter {

class Window;

class GLContext {
   public:
    explicit GLContext(const Window& window);
    ~GLContext();
    GLContext(const GLContext&) = delete;
    GLContext& operator=(const GLContext&) = delete;

   private:
    SDL_GLContext m_handle;
};

}  // namespace opticrafter