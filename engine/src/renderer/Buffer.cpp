#include "opticrafter/Buffer.h"

#include <glad/gl.h>

namespace opticrafter {

// ============================================================================
// Vertex buffer
// ============================================================================
VertexBuffer::VertexBuffer(std::span<const std::byte> data) {
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, data.size_bytes(), data.data(), GL_DYNAMIC_STORAGE_BIT);
}

VertexBuffer::~VertexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : m_id(other.m_id) { other.m_id = 0; }

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteBuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void VertexBuffer::updateSubData(size_t offset, std::span<const std::byte> data) {
    glNamedBufferSubData(m_id, offset, data.size_bytes(), data.data());
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// ============================================================================
// Index buffer
// ============================================================================
IndexBuffer::IndexBuffer(std::span<const uint32_t> data) {
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, data.size_bytes(), data.data(), GL_STATIC_DRAW);
    m_count = data.size();
}

IndexBuffer::~IndexBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : m_id(other.m_id) { other.m_id = 0; }

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteBuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

// ============================================================================
// Shader Storage Buffer (SSBO)
// ============================================================================
ShaderStorageBuffer::ShaderStorageBuffer() { glCreateBuffers(1, &m_id); }

ShaderStorageBuffer::~ShaderStorageBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept : m_id(other.m_id) { other.m_id = 0; }

ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteBuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void ShaderStorageBuffer::setData(std::span<const std::byte> data) {
    glNamedBufferData(m_id, data.size_bytes(), data.data(), GL_DYNAMIC_DRAW);
}

void ShaderStorageBuffer::bind(uint32_t binding_point) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, m_id);
}

void ShaderStorageBuffer::unbind(uint32_t binding_point) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, 0);
}

// ============================================================================
// Indirect Buffer
// ============================================================================
IndirectBuffer::IndirectBuffer() { glCreateBuffers(1, &m_id); }

IndirectBuffer::~IndirectBuffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}

IndirectBuffer::IndirectBuffer(IndirectBuffer&& other) noexcept : m_id(other.m_id) { other.m_id = 0; }

IndirectBuffer& IndirectBuffer::operator=(IndirectBuffer&& other) noexcept {
    if (this != &other) {
        if (m_id) glDeleteBuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void IndirectBuffer::setData(std::span<const std::byte> data) {
    glNamedBufferData(m_id, data.size_bytes(), data.data(), GL_DYNAMIC_DRAW);
}

void IndirectBuffer::bind() const { glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_id); }

void IndirectBuffer::unbind() const { glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0); }

}  // namespace opticrafter