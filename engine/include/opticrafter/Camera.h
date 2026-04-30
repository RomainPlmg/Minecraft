#pragma once

#include <glm/glm.hpp>

#include "opticrafter/Utils.h"

namespace opticrafter {

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
constexpr float CAMERA_YAW = -90.0f;
constexpr float CAMERA_PITCH = 0.0f;
constexpr float CAMERA_SENSITIVITY = 0.52f;
constexpr float CAMERA_ZOOM = 45.0f;

class Camera {
   public:
    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
           float yaw = CAMERA_YAW, float pitch = CAMERA_PITCH);

    glm::mat4 getProjection(const Viewport& viewport) const;
    glm::mat4 getView() const;
    glm::vec3 getFrontVector() const { return m_front; }
    glm::vec3 getRightVector() const { return m_right; }
    glm::vec3 getPosition() const;

    void setPosition(glm::vec3 position) { m_position = position; }
    void move(glm::vec3 movement) { m_position += movement; }
    void update();
    void freeze(bool freeze) { m_freeze = freeze; }

   private:
    void updateCameraVector();

    // Camera Attributes
    glm::vec3 m_position{0.f};
    glm::vec3 m_front{0.f};
    glm::vec3 m_up{0.f};
    glm::vec3 m_right{0.f};
    glm::vec3 m_world_up{0.f};
    glm::vec2 m_last_mouse_pos{0.f};

    // Euler Angles
    float m_yaw = CAMERA_YAW;
    float m_pitch = CAMERA_PITCH;

    // Camera options
    float m_sensitivity = CAMERA_SENSITIVITY;
    float m_zoom = CAMERA_ZOOM;
    float m_fov = 45.f;
    float m_near = 0.1f, m_far = 100.f;
    bool m_freeze = false;
};

}  // namespace opticrafter