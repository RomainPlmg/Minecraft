#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/Core.hpp"

namespace Engine {

class ENGINE_API Shader {
   public:
    Shader() = default;
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    /* Utility uniform functions */
    virtual void SetUniformInt(const std::string& name, int value) const = 0;
    virtual void SetUniformIntArray(const std::string& name, int* values, uint32_t count) const = 0;
    virtual void SetUniformFloat(const std::string& name, float value) const = 0;
    virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) const = 0;
    virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) const = 0;
    virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) const = 0;
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) const = 0;

    static Shader* Create(const std::string& name, const std::string& vertexPath,
                          const std::string& fragmentPath);
};

class ENGINE_API ShaderLibrary {
   public:
    static void Create(const std::string& name, const std::string& vertexPath,
                       const std::string& fragmentPath);
    static Shader* GetShader(const std::string& name);

   private:
    static std::unordered_map<std::string, Shader*> m_Shaders;
};

}  // namespace Engine