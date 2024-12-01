#pragma once

#include "Buffer.hpp"

namespace Engine {

class VertexArray {
   public:
    VertexArray() = default;
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(VertexBuffer* buffer) = 0;
    virtual void UpdateElementBuffer(ElementBuffer* buffer) = 0;

    static VertexArray* Create();

   protected:
    uint32_t m_VertexBufferIndex = 0;
    std::vector<VertexBuffer*> m_VertexBuffers;
    ElementBuffer* m_ElementBuffer;
};

}  // namespace Engine