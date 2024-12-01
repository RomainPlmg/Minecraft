#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/Core.hpp"

namespace Engine {

struct ENGINE_API TextureInfo {
    struct ENGINE_API UV {
        float uMin, uMax, vMin, vMax;

    } uv;
    int width, height, nbChannels;
};

class ENGINE_API Texture {
   public:
    Texture(const std::string& path, const std::string& name);
    ~Texture();

    void CleanTextureData();

    inline TextureInfo::UV GetAtlasUV() const { return m_TextureInfo.uv; }
    inline int GetWidth() const { return m_TextureInfo.width; }
    inline int GetHeight() const { return m_TextureInfo.height; }
    inline unsigned char* GetData() const { return m_Data; }
    inline std::string GetName() const { return m_Name; }

    inline void SetAtlasUV(const TextureInfo::UV& uv) { m_TextureInfo.uv = uv; }

   private:
    std::string m_Name;
    unsigned char* m_Data;
    TextureInfo m_TextureInfo;
};

}  // namespace Engine
