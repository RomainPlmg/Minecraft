#pragma once

#include <glm/glm.hpp>

#include "AABB.h"

namespace opticrafter {

class Frustum {
   public:
    enum Planes {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far,
    };

    void update(const glm::mat4& view_proj);
    bool intersects(const AABB& box) const;

   private:
    std::array<glm::vec4, 6> m_planes;

    static float distanceToPlane(const glm::vec4& plane, const glm::vec3& point);
};

}  // namespace opticrafter