#pragma once

#include <glm/glm.hpp>

namespace opticrafter {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
};

}  // namespace opticrafter