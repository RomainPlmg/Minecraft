#pragma once

#include "Engine/Renderer/Buffer.hpp"
#include "Engine/Renderer/VertexArray.hpp"

typedef unsigned int GLenum;

namespace Engine {

class OpenGLVertexArray : public VertexArray {
   public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    static GLenum GetOpenGLType(ShaderDataType type);

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(VertexBuffer* buffer) override;
    virtual void UpdateElementBuffer(ElementBuffer* buffer) override;

   private:
    uint32_t m_RendererID;
};

}  // namespace Engine
