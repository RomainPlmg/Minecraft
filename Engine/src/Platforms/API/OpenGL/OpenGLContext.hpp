#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Include GLFW after glad
// clang-format on

#include "Engine/Renderer/GraphicContext.hpp"

namespace Engine {

class OpenGLContext : public GraphicContext {
   public:
    OpenGLContext(GLFWwindow* windowHandler);

    virtual void Init() override;
    virtual void SwapBuffers() override;

   private:
    GLFWwindow* m_Handler;
};

}  // namespace Engine