#pragma once

#include <SDL3/SDL.h>

#include <concepts>
#include <memory>

#include "Engine.h"
#include "LayerStack.h"

namespace opticrafter {

class Layer {
   public:
    Layer(LayerStack* stack, Engine& engine);
    virtual ~Layer() = default;

    virtual void onEvent(SDL_Event& event) = 0;

    virtual void onUpdate(float dt) = 0;
    virtual void onRender() = 0;

    template <std::derived_from<Layer> T, typename... Args>
    void transitionTo(Args&&... args) {
        m_stack->queueCommand([this, ... args = std::forward<Args>(args)]() mutable {
            m_stack->replaceTop(std::make_unique<T>(m_stack, m_engine, std::forward<Args>(args)...));
        });
    }

    template <std::derived_from<Layer> T, typename... Args>
    void pushOverlay(Args&&... args) {
        m_stack->queueCommand([this, ... args = std::forward<Args>(args)]() mutable {
            m_stack->pushLayer(std::make_unique<T>(m_stack, m_engine, std::forward<Args>(args)...));
        });
    }

    void popSelf();

   protected:
    Engine& m_engine;
    LayerStack* m_stack;
};

}  // namespace opticrafter