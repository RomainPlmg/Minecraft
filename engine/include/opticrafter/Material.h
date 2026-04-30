#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <variant>

namespace opticrafter {

using ShaderID = uint32_t;
using TextureID = uint32_t;
using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, TextureID>;

struct Material {
    ShaderID id;
    std::unordered_map<std::string, UniformValue> uniforms{};
};

}  // namespace opticrafter