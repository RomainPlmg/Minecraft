#include "Shader.hpp"

#include "Engine/Core/Log.hpp"
#include "Platforms/API/OpenGL/OpenGLShader.hpp"
#include "RendererAPI.hpp"

namespace Engine {

// Shader
Shader* Shader::Create(const std::string& name, const std::string& vertexPath,
                       const std::string& fragmentPath) {
    switch (RendererAPI::GetAPI()) {
        case GraphicAPI::None:
            CORE_FATAL("No selected graphic API.");
            break;
        case GraphicAPI::OpenGL:
            return new OpenGLShader(name, vertexPath, fragmentPath);
            break;
        case GraphicAPI::Vulkan:
            CORE_FATAL("Vulkan is not actually supported");
            break;
        default:
            break;
    }

    CORE_FATAL("Unknown API.");
    return nullptr;
}

// Shader Library
std::vector<Shader*> ShaderLibrary::m_Shaders;

void ShaderLibrary::Create(const std::string& name, const std::string& vertexPath,
                           const std::string& fragmentPath) {
    m_Shaders.emplace_back(Shader::Create(name, vertexPath, fragmentPath));
}

Shader* ShaderLibrary::GetShader(const std::string& name) {
    for (const auto& shader : m_Shaders) {
        if (shader->GetName() == name) {
            return shader;
        }
    }

    CORE_ERROR("Shader '{0}' does not exists.", name);
    return nullptr;
}

}  // namespace Engine