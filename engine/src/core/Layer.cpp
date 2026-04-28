#include "opticrafter/Layer.h"

namespace opticrafter {

Layer::Layer(LayerStack* stack) : m_stack(stack) {}

void Layer::popSelf() {
    m_stack->queueCommand([this]() { m_stack->popLayer(); });
}

}  // namespace opticrafter