#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace opticrafter {

class GLContext;

using ShaderID = uint32_t;

class ShaderManager {
   public:
    explicit ShaderManager(const GLContext& ctx);
    ~ShaderManager();

    ShaderID loadFromSource(const std::string& vsh_src, const std::string& fsh_src);
    ShaderID loadFromFile(const std::string& vsh_path, const std::string& fsh_path);
    void bind(ShaderID id);

    void setInt(ShaderID id, const std::string& name, int value);
    void setFloat(ShaderID id, const std::string& name, float value);
    void setVec2(ShaderID id, const std::string& name, glm::vec2& value);
    void setVec3(ShaderID id, const std::string& name, glm::vec3& value);
    void setMat4(ShaderID id, const std::string& name, glm::mat4& value);

   private:
    std::vector<uint32_t> m_shaders;
};

}  // namespace opticrafter