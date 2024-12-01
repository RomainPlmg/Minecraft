#include "OpenGLContext.hpp"

#include "Engine/Core/Log.hpp"

namespace Engine {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandler) : m_Handler(windowHandler) {}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_Handler);                                  // Make OpenGL context
    int status = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));  // Initialized glad
    CORE_ASSERT(status, "Unable to initialize glad...");
    CORE_INFO("OpenGL initialized");
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_Handler); }

}  // namespace Engine