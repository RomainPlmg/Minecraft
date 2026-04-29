#pragma once

#include <memory>
#include <string>

#include "Mesh.h"
#include "ShaderManager.h"
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
    ShaderID createShaderFromFile(const std::string& vsh_path, const std::string& fsh_path);

    void setClearColor(const Color& color);
    void setViewport(const Viewport& viewport);
    void bindShader(ShaderID id);
    void draw(const Mesh& mesh);

   private:
    Viewport m_viewport;
    Color m_clear_color;
    std::unique_ptr<TextureManager> m_texture_manager;
    std::unique_ptr<ShaderManager> m_shader_manager;
};

}  // namespace opticrafter