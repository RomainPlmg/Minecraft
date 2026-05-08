#pragma once

#include <cstdint>
#include <span>

namespace opticrafter {

class VertexBuffer;
class IndexBuffer;

struct VertexAttrib {
    uint32_t index;
    int count;
    uint32_t type;
    size_t offset;
};

class VertexArray {
   public:
    VertexArray();
    ~VertexArray();
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind() const;
    void unbind() const;
    void setLayout(const VertexBuffer& vbo, const IndexBuffer& ebo, std::span<const VertexAttrib> attributes,
                   size_t stride);

   private:
    uint32_t m_id = 0;
};

}  // namespace opticrafter