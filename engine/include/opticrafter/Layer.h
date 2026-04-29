#pragma once

#include <concepts>
#include <memory>

#include "LayerStack.h"

namespace opticrafter {

class Layer {
   public:
    Layer(LayerStack* stack);
    virtual ~Layer() = default;

    // virtual void onEvent(Event& event) = 0; -> TODO

    virtual void onUpdate(float dt) = 0;
    virtual void onRender() = 0;

    template <std::derived_from<Layer> T, typename... Args>
    void transitionTo(Args&&... args) {
        m_stack->queueCommand([this, ... args = std::forward<Args>(args)]() mutable {
            m_stack->replaceTop(std::make_unique<T>(m_stack, std::forward<Args>(args)...));
        });
    }

    template <std::derived_from<Layer> T, typename... Args>
    void pushOverlay(Args&&... args) {
        m_stack->queueCommand([this, ... args = std::forward<Args>(args)]() mutable {
            m_stack->pushLayer(std::make_unique<T>(m_stack, std::forward<Args>(args)...));
        });
    }

    void popSelf();

   protected:
    LayerStack* m_stack;
};

}  // namespace opticrafter