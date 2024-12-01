#include "Buffer.hpp"

#include "Engine/Core/Log.hpp"
#include "Platforms/API/OpenGL/OpenGLBuffer.hpp"
#include "RendererAPI.hpp"

namespace Engine {

VertexBuffer* VertexBuffer::Create(uint32_t size) {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case GraphicAPI::OpenGL:
            return new OpenGLVertexBuffer(size);
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

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case GraphicAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
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

ElementBuffer* ElementBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case GraphicAPI::OpenGL:
            return new OpenGLElementBuffer(indices, count);
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

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_Stride(0), m_Elements{elements} {
    uint32_t offset = 0;
    for (auto& element : m_Elements) {
        element.Offset = offset;
        offset += static_cast<uint32_t>(element.Size);
        m_Stride += static_cast<uint32_t>(element.Size);
    }
}

}  // namespace Engine