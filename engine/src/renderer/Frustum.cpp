#include "opticrafter/Frustum.h"

namespace opticrafter {

void Frustum::update(const glm::mat4& view_proj) {
    // Left plane
    m_planes[Left].x = view_proj[0][3] + view_proj[0][0];
    m_planes[Left].y = view_proj[1][3] + view_proj[1][0];
    m_planes[Left].z = view_proj[2][3] + view_proj[2][0];
    m_planes[Left].w = view_proj[3][3] + view_proj[3][0];

    // Right plane
    m_planes[Right].x = view_proj[0][3] - view_proj[0][0];
    m_planes[Right].y = view_proj[1][3] - view_proj[1][0];
    m_planes[Right].z = view_proj[2][3] - view_proj[2][0];
    m_planes[Right].w = view_proj[3][3] - view_proj[3][0];

    // Bottom plane
    m_planes[Bottom].x = view_proj[0][3] + view_proj[0][1];
    m_planes[Bottom].y = view_proj[1][3] + view_proj[1][1];
    m_planes[Bottom].z = view_proj[2][3] + view_proj[2][1];
    m_planes[Bottom].w = view_proj[3][3] + view_proj[3][1];

    // Top plane
    m_planes[Top].x = view_proj[0][3] - view_proj[0][1];
    m_planes[Top].y = view_proj[1][3] - view_proj[1][1];
    m_planes[Top].z = view_proj[2][3] - view_proj[2][1];
    m_planes[Top].w = view_proj[3][3] - view_proj[3][1];
    // Near plane
    m_planes[Near].x = view_proj[0][3] + view_proj[0][2];
    m_planes[Near].y = view_proj[1][3] + view_proj[1][2];
    m_planes[Near].z = view_proj[2][3] + view_proj[2][2];
    m_planes[Near].w = view_proj[3][3] + view_proj[3][2];

    // Far plane
    m_planes[Far].x = view_proj[0][3] - view_proj[0][2];
    m_planes[Far].y = view_proj[1][3] - view_proj[1][2];
    m_planes[Far].z = view_proj[2][3] - view_proj[2][2];
    m_planes[Far].w = view_proj[3][3] - view_proj[3][2];

    // Normalize each plane
    for (auto& plane : m_planes) {
        const float length = glm::length(glm::vec3(plane));
        plane /= length;
    }
}

bool Frustum::intersects(const AABB& box) const {
    // Foreach plane of the frustum
    for (const auto& plane : m_planes) {
        // Find the point of the box furthest in the direction of the plane's normal
        glm::vec3 positive_vertex = box.min;

        if (plane.x >= 0) positive_vertex.x = box.max.x;
        if (plane.y >= 0) positive_vertex.y = box.max.y;
        if (plane.z >= 0) positive_vertex.z = box.max.z;

        // If the positive vertex is behind the plane, the box is completely outside
        if (distanceToPlane(plane, positive_vertex) < 0) {
            return false;
        }
    }

    // The box is at least partially visible
    return true;
}

float Frustum::distanceToPlane(const glm::vec4& plane, const glm::vec3& point) {
    return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
}

}  // namespace opticrafter