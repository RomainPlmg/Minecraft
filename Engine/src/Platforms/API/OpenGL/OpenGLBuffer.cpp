#include "OpenGLBuffer.hpp"

#include "OpenGLContext.hpp"

namespace Engine {

/* Vertex buffer */
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLVertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void OpenGLVertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

/* Element buffer */
OpenGLElementBuffer::OpenGLElementBuffer(uint32_t* indices, uint32_t count) {
    m_Count = count;
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
}

OpenGLElementBuffer::~OpenGLElementBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLElementBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void OpenGLElementBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace Engine