#pragma once

namespace Engine {

enum class GraphicAPI { None, OpenGL, Vulkan };
class Color;

// Pure virtual class, will be inherited by the renderer specific API
class RendererAPI {
   public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void SetClearColor(const Color& color) = 0;
    virtual void Clear() = 0;
    virtual void SetViewport(uint32_t width, uint32_t height) = 0;
    virtual void Draw(uint32_t count) = 0;

    /* Getters */
    static inline GraphicAPI GetAPI() { return m_API; }

    static RendererAPI* Create();

   private:
    static GraphicAPI m_API;
};

}  // namespace Engine