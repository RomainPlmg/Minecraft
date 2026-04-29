#include "opticrafter/Buffer.h"

#include <glad/gl.h>

namespace opticrafter {

// ============================================================================
// Vertex buffer
// ============================================================================
VertexBuffer::VertexBuffer(std::span<const std::byte> data) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// ============================================================================
// Index buffer
// ============================================================================
IndexBuffer::IndexBuffer(std::span<const uint32_t> data) {
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
    m_count = data.size();
}

IndexBuffer::~IndexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace opticrafter