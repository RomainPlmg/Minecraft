#pragma once

#include "Engine/Renderer/Buffer.hpp"

namespace Engine {

class OpenGLVertexBuffer : public VertexBuffer {
   public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    ~OpenGLVertexBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual inline const BufferLayout& GetLayout() const override { return m_Layout; }
    virtual inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

   private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

class OpenGLElementBuffer : public ElementBuffer {
   public:
    OpenGLElementBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLElementBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

   private:
    uint32_t m_RendererID;
};

}  // namespace Engine