#pragma once

#include "Engine/Renderer/RendererAPI.hpp"

namespace Engine {

class Color;

class OpenGLRendererAPI : public RendererAPI {
   public:
    OpenGLRendererAPI() = default;
    virtual void Init() override;
    virtual void SetClearColor(const Color& color) override;
    virtual void Clear() override;
    virtual void SetViewport(uint32_t width, uint32_t height) override;
    virtual void Draw(uint32_t count) override;

   private:
};

}  // namespace Engine