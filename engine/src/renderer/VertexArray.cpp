#include "opticrafter/VertexArray.h"

#include <glad/gl.h>

#include <stdexcept>

#include "opticrafter/Buffer.h"

namespace opticrafter {

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_id); }

VertexArray::~VertexArray() {
    if (m_id) glDeleteVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept : m_id(other.m_id) { other.m_id = 0; }

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteVertexArrays(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::setLayout(const VertexBuffer& vbo, const IndexBuffer& ebo, std::span<const VertexAttrib> attributes,
                            size_t stride) {
    for (auto& a : attributes) {
        if (a.count > 4) {
            throw std::runtime_error(
                "Vertex attribute must have a size < 4. See https://docs.gl/gl4/glVertexAttribPointer.");
        }
        glEnableVertexArrayAttrib(m_id, a.index);
        glVertexArrayAttribFormat(m_id, a.index, a.count, a.type, GL_FALSE, a.offset);
        glVertexArrayAttribBinding(m_id, a.index, 0);
    }

    glVertexArrayVertexBuffer(m_id, 0, vbo.id(), 0, stride);
    glVertexArrayElementBuffer(m_id, ebo.id());
}

}  // namespace opticrafter