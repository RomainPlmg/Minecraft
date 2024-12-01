#include "WindowsInput.hpp"

#include "Platforms/API/OpenGL/OpenGLContext.hpp"

namespace Engine {

#if defined(ENG_PLATFORM_WINDOWS)
Input* Input::m_Instance = new WindowsInput();
#endif

bool WindowsInput::IsKeyPressedImpl(KeyCode keycode) {
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetHandler());

    auto state = glfwGetKey(window, keycode);
    return (state == GLFW_PRESS || state == GLFW_REPEAT);
}

bool WindowsInput::IsMouseButtonPressedImpl(MouseButtonCode button) {
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetHandler());

    auto state = glfwGetMouseButton(window, button);
    return (state == GLFW_PRESS || state == GLFW_REPEAT);
}

glm::vec2 WindowsInput::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetHandler());

    glm::dvec2 position;
    glfwGetCursorPos(window, &position.x, &position.y);

    return static_cast<glm::vec2>(position);
}

}  // namespace Engine