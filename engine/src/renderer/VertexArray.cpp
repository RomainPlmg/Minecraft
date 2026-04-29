#include "opticrafter/VertexArray.h"

#include <glad/gl.h>

#include <stdexcept>

#include "opticrafter/Buffer.h"

namespace opticrafter {

VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::~VertexArray() {
    if (m_id) glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::setLayout(const VertexBuffer& vbo, std::span<const VertexAttrib> attributes, size_t stride) {
    bind();
    vbo.bind();

    for (auto& a : attributes) {
        if (a.count > 4) {
            throw std::runtime_error(
                "Vertex attribute must have a size < 4. See https://docs.gl/gl4/glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(a.index);
        glVertexAttribPointer(a.index, a.count, a.type, GL_FALSE, stride, (void*)a.offset);
    }
}

}  // namespace opticrafter