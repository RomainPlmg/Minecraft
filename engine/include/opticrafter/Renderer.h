#pragma once

#include "Utils.h"

namespace opticrafter {

class GLContext;

class Renderer {
   public:
    explicit Renderer(const GLContext& ctx);
    ~Renderer();

    void clear();

    void setClearColor(const Color& color);
    void setViewport(const Viewport& viewport);

   private:
    Viewport m_viewport;
    Color m_clear_color;
};

}  // namespace opticrafter