#pragma once

namespace Engine {

class Shader;
class Texture;

class Drawable {
   public:
    Drawable();
    virtual ~Drawable() = default;

    inline std::vector<float>& GetVertices() { return m_Vertices; }

    static inline std::vector<Drawable*>& GetArray() { return m_DrawableArray; }

   private:
    static std::vector<Drawable*> m_DrawableArray;

    Texture* m_Texture;
    Shader* m_Shader;
    std::vector<float> m_Vertices;
};

}  // namespace Engine