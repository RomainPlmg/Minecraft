#include "Window.hpp"

#include "Engine/Events/EventApplication.hpp"
#include "Log.hpp"
#include "Platforms/Linux/LinuxWindow.hpp"
#include "Platforms/Windows/WindowsWindow.hpp"

namespace Engine {

Window* Window::Create(const WindowProps& props) {
#if defined(ENG_PLATFORM_WINDOWS)
    return new WindowsWindow(props);
#elif defined(ENG_PLATFORM_LINUX)
    return new LinuxWindow(props);
#else
    CORE_FATAL("Unsupported platform. Abort.");
#endif
}

WindowProps::WindowProps(std::string title, uint32_t width, uint32_t height)
    : width(width), height(height), title(title), vsync(true) {}

Window::Window() { m_EventDispatcher = new EventDispatcher(); }

Window::~Window() {
    delete m_GraphicContext;
    delete m_EventDispatcher;
}

void Window::OnEvent(const Event& event) {
    if (event.GetType() == EventType::WindowResize) {
        const WindowResizeEvent* wrEvent = dynamic_cast<const WindowResizeEvent*>(&event);
        m_Props.width = wrEvent->width;
        m_Props.height = wrEvent->height;
    }
}

}  // namespace Engine
