#pragma once

#include <cstdint>
#include <span>

namespace opticrafter {

class VertexBuffer;

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

    void bind() const;
    void unbind() const;
    void setLayout(const VertexBuffer& vbo, std::span<VertexAttrib> attributes, size_t stride);

   private:
    uint32_t m_id = 0;
};

}  // namespace opticrafter