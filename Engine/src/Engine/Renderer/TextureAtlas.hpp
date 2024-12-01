#pragma once

#include "Texture.hpp"

namespace Engine {

class TextureAtlas {
   public:
    TextureAtlas() = default;
    virtual ~TextureAtlas();

    virtual void Init() = 0;
    virtual void Bind() = 0;

    inline Texture* GetTexture(const std::string& textureName) const {
        return m_TextureArray.at(textureName);
    }

    static TextureAtlas* Create();

   protected:
    int m_Width = 0, m_Height = 0, m_Resolution = 0;
    std::unordered_map<std::string, Texture*> m_TextureArray;
    void LoadJSON(const std::string& path);
};

}  // namespace Engine