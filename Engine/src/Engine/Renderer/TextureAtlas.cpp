#include "TextureAtlas.hpp"

#include <nlohmann/json.hpp>

#include "Engine/Core/Log.hpp"
#include "Engine/Utils/Utils.hpp"
#include "Platforms/API/OpenGL/OpenGLTextureAtlas.hpp"
#include "RendererAPI.hpp"
#include "Texture.hpp"

namespace Engine {

using json = nlohmann::json;

TextureAtlas::~TextureAtlas() {
    for (auto& pair : m_TextureArray) {
        delete pair.second;
    }
    m_TextureArray.clear();
}

TextureAtlas* TextureAtlas::Create() {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
        case GraphicAPI::OpenGL:
            return new OpenGLTextureAtlas();
        case GraphicAPI::Vulkan:
            CORE_FATAL("Vulkan is not actually supported");
        default:
            break;
    }

    CORE_FATAL("Unknown API.");
    return nullptr;
}

void TextureAtlas::LoadJSON(const std::string& path) {
    std::string jsonContent = Utils::ReadFromFile(path);

    json data = json::parse(jsonContent);

    for (const auto& item : data) {
        std::string source = item["source"];
        std::string texturePath = PROJECT_SOURCE_DIR "Minecraft/assets/textures/" + source;

        for (const auto& entry : std::filesystem::directory_iterator(texturePath)) {
            // Find the last slash or backslash
            size_t lastSlash = entry.path().string().find_last_of("/\\");
            // Extract the filename with extension
            std::string filename = entry.path().string().substr(lastSlash + 1);
            // Find the dot before the extension
            size_t dotPos = filename.find_last_of('.');
            // Extract the word before the dot
            std::string textureName = filename.substr(0, dotPos);

            Texture* texture = new Texture(entry.path().string(), textureName);

            m_TextureArray[texture->GetName()] = texture;
            m_Resolution +=
                m_TextureArray[textureName]->GetWidth() * m_TextureArray[textureName]->GetHeight();
        }

        // Initial estimate: the square root of the number of pixels
        int estimate = static_cast<int>(std::sqrt(m_Resolution));

        // Find the largest multiple of 16 less than or equal to the estimate
        m_Width = (estimate / 16) * 16;
        m_Height = m_Resolution / m_Width;

        // Adjust the dimensions until height is also a multiple of 16
        while (m_Height % 16 != 0) {
            m_Width -= 16;
            m_Height = m_Resolution / m_Width;
        }
    }
}

}  // namespace Engine
