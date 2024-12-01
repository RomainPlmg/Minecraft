#include "OpenGLRendererAPI.hpp"

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Utils/Color.hpp"
#include "OpenGLContext.hpp"

namespace Engine {

// Define the message callback function
static void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity,
                                  int length, const char* message, const void* userParam) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            CORE_FATAL(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            CORE_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            CORE_WARN(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            CORE_TRACE(message);
            return;
    }

    CORE_ASSERT(false, "Unknown severity level");
}

void OpenGLRendererAPI::Init() {
#ifdef ENG_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);  // Activate debug messages from OpenGL
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
                          GL_FALSE);
#endif

    glEnable(GL_BLEND);  // Activate blending for object transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);  // Activate the depth buffer
    glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::SetClearColor(const Color& color) {
    // Color class set each RGBA between 0 & 255, OpenGL between 0 & 1
    Color glColor = color.ToGLColor();
    glClearColor(glColor.r, glColor.g, glColor.b, glColor.a);
}

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::SetViewport(uint32_t width, uint32_t height) {
    glViewport(0, 0, width, height);
}

void OpenGLRendererAPI::Draw(uint32_t count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Engine