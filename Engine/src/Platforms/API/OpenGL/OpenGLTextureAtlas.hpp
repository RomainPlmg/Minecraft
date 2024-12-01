#pragma once

#include "Engine/Renderer/TextureAtlas.hpp"

namespace Engine {

class OpenGLTextureAtlas : public TextureAtlas {
   public:
    OpenGLTextureAtlas();

    virtual void Init() override;
    virtual void Bind() override;

   private:
    uint32_t m_RendererID;
};

}  // namespace Engine
