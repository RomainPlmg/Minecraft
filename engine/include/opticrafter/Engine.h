#pragma once
#include <memory>

#include "EventBus.h"
#include "LayerStack.h"
#include "Renderer.h"
#include "Timer.h"
#include "Window.h"

namespace opticrafter {

class Engine {
   public:
    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void run();

    [[nodiscard]] Renderer* renderer() { return m_renderer.get(); }
    [[nodiscard]] EventBus* eventBus() { return &m_event_bus; }

   private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    Timer m_timer;
    LayerStack m_layer_stack;
    EventBus m_event_bus;
};

}  // namespace opticrafter