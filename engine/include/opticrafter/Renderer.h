#pragma once

#include <memory>
#include <string>

#include "TextureManager.h"
#include "Utils.h"


namespace opticrafter {

class GLContext;

class Renderer {
   public:
    explicit Renderer(const GLContext& ctx);
    ~Renderer();

    void clear();

    TextureID createTexture(const std::string& path);

    void setClearColor(const Color& color);
    void setViewport(const Viewport& viewport);

   private:
    Viewport m_viewport;
    Color m_clear_color;
    std::unique_ptr<TextureManager> m_texture_manager;
};

}  // namespace opticrafter