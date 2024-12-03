#include "Application.hpp"

#include "Engine/Events/Event.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Utils/Color.hpp"
#include "Log.hpp"

namespace Engine {

Application* Application::m_Instance = nullptr;

Application::Application() : m_Renderer(nullptr), m_Window(nullptr) {
    if (m_Instance == nullptr) {
        m_Instance = this;
    } else {
        CORE_WARN("Application already exists.");
        return;
    }

    m_Window = Window::Create();
    m_Renderer = Renderer::Create();
}

Application::~Application() {
    delete m_Window;
    delete m_Renderer;
}

void Application::Run() {
    m_Window->GetEventDispatcher()->Subscribe(EventCategory::EventCategoryAll,
                                              BIND_EVENT_FN(OnEvent));
    m_Renderer->Init();

    m_Renderer->SetClearColor(Color(80.0f, 120.0f, 200.0f));
    while (!m_Window->ShouldClose()) {
        m_Renderer->Clear();
        m_Renderer->Draw();
        m_Window->Update();
    }

    m_Window->Shutdown();
    CORE_INFO("Engine exit successful!");
}

void Application::OnEvent(const Event& event) { CORE_TRACE(event.ToString()); }

}  // namespace Engine