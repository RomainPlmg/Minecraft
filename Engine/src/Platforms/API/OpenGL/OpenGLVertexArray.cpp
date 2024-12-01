#include "OpenGLVertexArray.hpp"

#include "Engine/Core/Log.hpp"
#include "Engine/Renderer/Buffer.hpp"
#include "OpenGLContext.hpp"

namespace Engine {

OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &m_RendererID); }

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

GLenum OpenGLVertexArray::GetOpenGLType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::None:
            return GL_NONE;
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            break;
    }
    CORE_ERROR("Unkmown shader data type");
    return 0;
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* buffer) {
    glBindVertexArray(m_RendererID);
    buffer->Bind();

    const auto& layout = buffer->GetLayout();
    for (const auto& element : layout.GetElements()) {
        if (element.Type == ShaderDataType::Float || element.Type == ShaderDataType::Float2 ||
            element.Type == ShaderDataType::Float3 || element.Type == ShaderDataType::Float4) {
            glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                                  GetOpenGLType(element.Type),
                                  element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                  (const GLvoid*)static_cast<uintptr_t>(element.Offset));
            glEnableVertexAttribArray(m_VertexBufferIndex);
        } else if (element.Type == ShaderDataType::Int || element.Type == ShaderDataType::Int2 ||
                   element.Type == ShaderDataType::Int3 || element.Type == ShaderDataType::Int4 ||
                   element.Type == ShaderDataType::Bool) {
            glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
                                   GetOpenGLType(element.Type), layout.GetStride(),
                                   (const GLvoid*)static_cast<uintptr_t>(element.Offset));
            glEnableVertexAttribArray(m_VertexBufferIndex);
        } else if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4) {
            // glEnableVertexAttribArray(m_VertexBufferIndex);
            CORE_WARN("Matrices in vertex arrays not yet supported");
        } else {
            CORE_ERROR("Unkmown shader data type");
        }
        m_VertexBufferIndex++;
    }

    m_VertexBuffers.push_back(buffer);
}

void OpenGLVertexArray::UpdateElementBuffer(ElementBuffer* buffer) {
    glBindVertexArray(m_RendererID);
    buffer->Bind();

    m_ElementBuffer = buffer;
}

}  // namespace Engine
