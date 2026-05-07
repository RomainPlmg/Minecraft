#include "opticrafter/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "opticrafter/Input.h"

namespace opticrafter {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    m_position = position;
    m_world_up = up;
    m_yaw = yaw;
    m_pitch = pitch;

    m_sensitivity = CAMERA_SENSITIVITY;
    m_zoom = CAMERA_ZOOM;

    updateCameraVector();
}

glm::mat4 Camera::projection(const Viewport& viewport) const {
    float aspect = static_cast<float>(viewport.w) / static_cast<float>(viewport.h);
    return glm::perspective(glm::radians(m_fov), aspect, m_near, m_far);
}

glm::mat4 Camera::view() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

glm::vec3 Camera::position() const { return m_position; }

void Camera::update(const Viewport& viewport) {
    glm::vec2 mouse_pos;
    Input::getMousePosition(&mouse_pos.x, &mouse_pos.y);
    auto mouse_offset = m_last_mouse_pos - mouse_pos;

    if (!m_freeze) {
        mouse_offset *= m_sensitivity;

        m_yaw -= mouse_offset.x;
        m_pitch += mouse_offset.y;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVector();
    }
    m_frustum.update(projection(viewport) * view());

    m_last_mouse_pos = mouse_pos;
}

void Camera::updateCameraVector() {
    // Calculate the new Front vector
    glm::vec3 front(0.0f);
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(
        glm::cross(m_front, m_world_up));  // Normalize the vectors, because their length gets closer to 0 the more you

    // Look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

}  // namespace opticrafter