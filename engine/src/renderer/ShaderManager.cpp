#include "opticrafter/ShaderManager.h"

#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

#include "opticrafter/Logger.h"
#include "opticrafter/Utils.h"

namespace opticrafter {

static GLuint compileStage(GLenum type, const std::string& src) {
    GLuint shader = glCreateShader(type);
    auto c_src = src.c_str();
    glShaderSource(shader, 1, &c_src, nullptr);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        LOG_CORE_ERROR("Shader compile error: {}", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint linkProgram(GLuint vert, GLuint frag) {
    if (!vert || !frag) {
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw std::runtime_error("Invalid shaders to build the program.");
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint ok;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        LOG_CORE_ERROR("Shader link error: {}", log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

static GLint getUniform(GLuint id, const std::string& name) {
    GLint loc = glGetUniformLocation(id, name.c_str());
    if (loc == -1) LOG_CORE_WARN("Uniform '{}' not found in shader.", name);
    return loc;
}

ShaderManager::ShaderManager(const GLContext& ctx) {
    (void)ctx;  // Guard, need a valid context to build shader manager
}

ShaderManager::~ShaderManager() {
    for (auto& id : m_shaders) {
        glDeleteProgram(id);
    }
}

ShaderID ShaderManager::loadFromSource(const std::string& vsh_src, const std::string& fsh_src) {
    if (vsh_src.empty() || fsh_src.empty()) {
        LOG_CORE_WARN("Shader is empty.");
        return 0;
    }

    GLuint vert = compileStage(GL_VERTEX_SHADER, vsh_src);
    GLuint frag = compileStage(GL_FRAGMENT_SHADER, fsh_src);

    if (!vert || !frag) {
        throw std::runtime_error("Shader compilation failed.");
    }

    GLuint program = linkProgram(vert, frag);
    if (!program) {
        throw std::runtime_error("Shader program link failed.");
    }

    m_shaders.push_back(program);

    LOG_CORE_TRACE("Shader compiled at ShaderID {}", m_shaders.size() - 1);

    return m_shaders.size() - 1;
}

ShaderID ShaderManager::loadFromFile(const std::string& vsh_path, const std::string& fsh_path) {
    std::string vsh_src = readFile(vsh_path);
    std::string fsh_src = readFile(fsh_path);

    if (vsh_src.empty() || fsh_src.empty()) {
        LOG_CORE_WARN("Shader is empty.");
        return 0;
    }

    GLuint vert = compileStage(GL_VERTEX_SHADER, vsh_src);
    GLuint frag = compileStage(GL_FRAGMENT_SHADER, fsh_src);

    if (!vert) {
        std::stringstream ss;
        ss << "Shader '" << vsh_path << "' compilation failed.";
        throw std::runtime_error(ss.str());
    }

    if (!frag) {
        std::stringstream ss;
        ss << "Shader '" << fsh_path << "' compilation failed.";
        throw std::runtime_error(ss.str());
    }

    GLuint program = linkProgram(vert, frag);
    if (!program) {
        std::stringstream ss;
        ss << "Shader program link failed:\n\t Vertex: '" << vsh_path << "' \n\t Fragment: '" << fsh_path << "'";
        throw std::runtime_error(ss.str());
    }

    m_shaders.push_back(program);

    LOG_CORE_TRACE("Shader compiled at ShaderID {}", m_shaders.size() - 1);

    return m_shaders.size() - 1;
}

void ShaderManager::bind(ShaderID id) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }

    glUseProgram(m_shaders[id]);
}

void ShaderManager::setInt(ShaderID id, const std::string& name, int value) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }
    glProgramUniform1i(m_shaders[id], getUniform(m_shaders[id], name), value);
}

void ShaderManager::setFloat(ShaderID id, const std::string& name, float value) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }
    glProgramUniform1f(m_shaders[id], getUniform(m_shaders[id], name), value);
}

void ShaderManager::setVec2(ShaderID id, const std::string& name, glm::vec2& value) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }
    glProgramUniform2fv(m_shaders[id], getUniform(m_shaders[id], name), 1, glm::value_ptr(value));
}

void ShaderManager::setVec3(ShaderID id, const std::string& name, glm::vec3& value) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }
    glProgramUniform3fv(m_shaders[id], getUniform(m_shaders[id], name), 1, glm::value_ptr(value));
}

void ShaderManager::setMat4(ShaderID id, const std::string& name, glm::mat4& value) {
    if (id >= m_shaders.size() || m_shaders[id] == 0) {
        LOG_CORE_ERROR("No valid shader at id '{}'", id);
        return;
    }
    glProgramUniformMatrix4fv(m_shaders[id], getUniform(m_shaders[id], name), 1, GL_FALSE, glm::value_ptr(value));
}

}  // namespace opticrafter