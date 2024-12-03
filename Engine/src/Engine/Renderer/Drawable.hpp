#pragma once

#include "Engine/Core/Core.hpp"

namespace Engine {

class Shader;
class Texture;

class ENGINE_API Drawable {
   public:
    Drawable() = default;
    virtual ~Drawable();

    inline std::vector<float>& GetVertices() { return m_Vertices; }
    inline Shader* GetShader() { return m_Shader; }
    inline Texture* GetTexture() { return m_Texture; }

    static inline std::vector<Drawable*>& GetArray() { return m_DrawableArray; }

   private:
    static std::vector<Drawable*> m_DrawableArray;

   protected:
    void Register();
    void Unregister();
    Texture* m_Texture = nullptr;
    Shader* m_Shader = nullptr;
    std::vector<float> m_Vertices;
};

}  // namespace Engine