#pragma once

namespace Engine {

// Virtual GraphicContext class, will be inherited by a specific API rendering context (OpenGL,
// Vulkan, etc.)
class GraphicContext {
   public:
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static GraphicContext* Create(void* window);
};

}  // namespace Engine