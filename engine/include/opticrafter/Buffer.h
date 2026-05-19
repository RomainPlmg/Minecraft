#pragma once

#include <cstdint>
#include <span>

namespace opticrafter {

class VertexBuffer {
   public:
    VertexBuffer(std::span<const std::byte> data);
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void updateSubData(size_t offset, std::span<const std::byte> data);
    void bind() const;
    void unbind() const;
    uint32_t id() const { return m_id; }

   private:
    uint32_t m_id = 0;
};

class IndexBuffer {
   public:
    IndexBuffer(std::span<const uint32_t> data);
    ~IndexBuffer();
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void bind() const;
    void unbind() const;
    uint32_t id() const { return m_id; }

    uint32_t count() const { return m_count; }

   private:
    uint32_t m_id = 0;
    uint32_t m_count = 0;
};

class ShaderStorageBuffer {
   public:
    ShaderStorageBuffer();
    ~ShaderStorageBuffer();

    ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept;
    ShaderStorageBuffer& operator=(ShaderStorageBuffer&& other) noexcept;

    void setData(std::span<const std::byte> data);
    void bind(uint32_t binding_point);
    void unbind(uint32_t binding_point);

    uint32_t id() const { return m_id; }

   private:
    uint32_t m_id = 0;
};

class IndirectBuffer {
   public:
    IndirectBuffer();
    ~IndirectBuffer();

    IndirectBuffer(IndirectBuffer&& other) noexcept;
    IndirectBuffer& operator=(IndirectBuffer&& other) noexcept;

    void setData(std::span<const std::byte> data);
    void bind() const;
    void unbind() const;

    uint32_t id() const { return m_id; }

   private:
    uint32_t m_id = 0;
};

}  // namespace opticrafter