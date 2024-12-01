#include "VertexArray.hpp"

#include "Engine/Core/Log.hpp"
#include "Platforms/API/OpenGL/OpenGLVertexArray.hpp"
#include "RendererAPI.hpp"

namespace Engine {

VertexArray* VertexArray::Create() {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
        case GraphicAPI::OpenGL:
            return new OpenGLVertexArray();
        case GraphicAPI::Vulkan:
            CORE_FATAL("Vulkan is not actually supported");
        default:
            break;
    }

    CORE_FATAL("Unknown API.");
    return nullptr;
}

}  // namespace Engine