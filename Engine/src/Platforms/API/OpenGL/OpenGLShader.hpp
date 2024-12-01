#pragma once

#include "Engine/Renderer/Shader.hpp"

namespace Engine {

class OpenGLShader : public Shader {
   public:
    OpenGLShader(const std::string& name, const std::string& vertexSrc,
                 const std::string& fragmentSrc);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    /* Utility uniform functions */
    virtual void SetUniformInt(const std::string& name, int value) const override;
    virtual void SetUniformIntArray(const std::string& name, int* values,
                                    uint32_t count) const override;
    virtual void SetUniformFloat(const std::string& name, float value) const override;
    virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) const override;
    virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) const override;
    virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) const override;
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) const override;

   private:
    static std::string ParseFile(const std::string& filePath);
    static uint32_t CompileShader(uint32_t type, const std::string& source);
    uint32_t m_RendererID;
};

}  // namespace Engine