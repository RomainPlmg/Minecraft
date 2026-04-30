#pragma once

#include <memory>
#include <string>

#include "Material.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Utils.h"

namespace opticrafter {

class GLContext;
class Camera;

struct SceneData {
    glm::mat4 view{1.f};
    glm::mat4 proj{1.f};
};

class Renderer {
   public:
    explicit Renderer(const GLContext& ctx);
    ~Renderer();

    void clear();

    void setClearColor(const Color& color);
    void setViewport(const Viewport& viewport);

    void beginScene(const Camera& camera);
    void draw(const Mesh& mesh, const Material& material, const glm::mat4& transform);

    Viewport viewport() const { return m_viewport; }
    TextureManager* textures() const { return m_texture_manager.get(); }
    ShaderManager* shaders() const { return m_shader_manager.get(); }

   private:
    Viewport m_viewport;
    Color m_clear_color;
    std::unique_ptr<TextureManager> m_texture_manager;
    std::unique_ptr<ShaderManager> m_shader_manager;
    SceneData m_scene_data;
};

}  // namespace opticrafter