#pragma once

#include <SDL3/SDL.h>

#include <memory>

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

struct RenderStats {
    uint32_t draw_calls = 0;
    uint32_t triangles = 0;
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
    SDL_Window* nativeWindow() const;
    RenderStats stats() const { return m_stats; }

   private:
    Viewport m_viewport;
    Color m_clear_color;
    std::unique_ptr<TextureManager> m_texture_manager;
    std::unique_ptr<ShaderManager> m_shader_manager;
    SceneData m_scene_data;
    RenderStats m_stats;
};

}  // namespace opticrafter