#pragma once

#include "Core.hpp"
#include "Window.hpp"

namespace Engine {

class Renderer;
class Event;

class ENGINE_API Application {
   public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(const Event& event);

    /* Getters */
    inline Window* GetWindow() const { return m_Window; }

    static inline Application* GetInstance() { return m_Instance; }

   private:
    Window* m_Window;
    Renderer* m_Renderer;

    static Application* m_Instance;
};

// To be defined by the client
Application* CreateApplication();

}  // namespace Engine