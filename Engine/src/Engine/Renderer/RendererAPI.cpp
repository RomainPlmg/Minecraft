#include "RendererAPI.hpp"

#include "Engine/Core/Log.hpp"
#include "Platforms/API/OpenGL/OpenGLRendererAPI.hpp"

namespace Engine {

// OpenGL is the default graphic API, chosen at the compile time
GraphicAPI RendererAPI::m_API = GraphicAPI::OpenGL;

RendererAPI* RendererAPI::Create() {
    switch (m_API) {
        case Engine::GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case Engine::GraphicAPI::OpenGL:
            return new OpenGLRendererAPI();
            break;
        case Engine::GraphicAPI::Vulkan:
            break;
        default:
            break;
    }

    CORE_FATAL("Unknown API.");
    return nullptr;
}

}  // namespace Engine