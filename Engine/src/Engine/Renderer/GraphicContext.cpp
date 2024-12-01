#include "GraphicContext.hpp"

#include "Engine/Core/Log.hpp"
#include "Platforms/API/OpenGL/OpenGLContext.hpp"
#include "RendererAPI.hpp"

namespace Engine {

GraphicContext* GraphicContext::Create(void* window) {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case GraphicAPI::OpenGL:
            return new OpenGLContext((GLFWwindow*)window);
            break;
        case GraphicAPI::Vulkan:
            CORE_FATAL("Vulkan is not actually supported");
            break;
        default:
            break;
    }

    CORE_FATAL("Unknown API.");
    return nullptr;
}

}  // namespace Engine