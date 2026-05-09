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
    void quit();

    template <typename TLayer, typename... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
    void pushLayer(Args&&... args) {
        m_layer_stack.pushLayer(std::make_unique<TLayer>(&m_layer_stack, *this, std::forward<Args>(args)...));
    }

    [[nodiscard]] Window* window() { return m_window.get(); }
    [[nodiscard]] Renderer* renderer() { return m_renderer.get(); }
    [[nodiscard]] EventBus* eventBus() { return &m_event_bus; }

   private:
    bool m_running = true;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    Timer m_timer;
    LayerStack m_layer_stack;
    EventBus m_event_bus;
};

}  // namespace opticrafter