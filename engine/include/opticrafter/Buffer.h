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

    void bind() const;
    void unbind() const;

   private:
    uint32_t m_id = 0;
};

class IndexBuffer {
   public:
    IndexBuffer(std::span<const std::byte> data);
    ~IndexBuffer();
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    void bind() const;
    void unbind() const;

    uint32_t count() const { return m_count; }

   private:
    uint32_t m_id = 0;
    uint32_t m_count = 0;
};

}  // namespace opticrafter