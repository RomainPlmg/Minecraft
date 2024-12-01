#include "WindowsWindow.hpp"

#include "Engine/Core/Log.hpp"
#include "Engine/Events/EventApplication.hpp"
#include "Engine/Events/EventKeyboard.hpp"
#include "Engine/Events/EventMouse.hpp"
#include "Platforms/API/OpenGL/OpenGLContext.hpp"

namespace Engine {

WindowsWindow::WindowsWindow(const WindowProps& props) {
    // GLFW initialization
    CORE_ASSERT(glfwInit(), "Unable to initialize GLFW...");

    // Create the window handler
    m_Handler = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    CORE_ASSERT(m_Handler, "The window handler is null...");

    glfwSetWindowUserPointer(m_Handler, this);

    /* Set callback functions */
    glfwSetFramebufferSizeCallback(m_Handler, framebuffer_size_callback);
    glfwSetKeyCallback(m_Handler, key_callback);
    glfwSetCursorPosCallback(m_Handler, mouse_cursor_pos_callback);
    glfwSetMouseButtonCallback(m_Handler, mouse_button_callback);
    glfwSetWindowPosCallback(m_Handler, window_pos_callback);

    m_GraphicContext = GraphicContext::Create(m_Handler);
    m_GraphicContext->Init();
}

void WindowsWindow::Update() {
    glfwPollEvents();  // Processes all pending events(keyboard, mouse, etc.),
                       //  calls the associated callbacks, and updates GLFW's internal state.
                       //  It must be called in the main loop to receive real-time events.
    m_GraphicContext->SwapBuffers();
}

void WindowsWindow::Close() { glfwSetWindowShouldClose(m_Handler, 1); }

int WindowsWindow::ShouldClose() { return glfwWindowShouldClose(m_Handler); }

void WindowsWindow::Shutdown() {
    glfwDestroyWindow(m_Handler);
    glfwTerminate();
}

void WindowsWindow::SetVSync(bool enable) {
    if (enable) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    m_Props.vsync = enable;
}

/* Callback functions */
void WindowsWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* self = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
    WindowResizeEvent event(width, height);
    self->m_EventDispatcher->Dispatch(event);
}

void WindowsWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* self = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
    KeyEvent* event = nullptr;
    switch (action) {
        case GLFW_PRESS:
            event = new KeyPressedEvent(static_cast<KeyCode>(key));
            self->m_EventDispatcher->Dispatch(*event);
            break;
        case GLFW_REPEAT:
            event = new KeyPressedEvent(static_cast<KeyCode>(key), true);
            self->m_EventDispatcher->Dispatch(*event);
            break;
        case GLFW_RELEASE:
            event = new KeyReleasedEvent(static_cast<KeyCode>(key));
            self->m_EventDispatcher->Dispatch(*event);
            break;
        default:
            break;
    }
    if (event != nullptr) {
        delete event;
    }
}

void WindowsWindow::mouse_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    auto* self = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
    MouseMotionEvent event(xpos, ypos);
    self->m_EventDispatcher->Dispatch(event);
}

void WindowsWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* self = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
    MouseEvent* event = nullptr;
    switch (action) {
        case GLFW_PRESS:
            event = new MouseButtonPressedEvent(static_cast<MouseButtonCode>(button));
            self->m_EventDispatcher->Dispatch(*event);
            break;
        case GLFW_RELEASE:
            event = new MouseButtonReleasedEvent(static_cast<MouseButtonCode>(button));
            self->m_EventDispatcher->Dispatch(*event);
            break;
        default:
            break;
    }
    if (event != nullptr) {
        delete event;
    }
}

void WindowsWindow::window_pos_callback(GLFWwindow* window, int xpos, int ypos) {
    auto* self = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
    WindowMovedEvent event(xpos, ypos);
    self->m_EventDispatcher->Dispatch(event);
}

}  // namespace Engine
