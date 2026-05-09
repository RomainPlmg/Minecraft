#include "opticrafter/Layer.h"

namespace opticrafter {

Layer::Layer(LayerStack* stack, Engine& engine) : m_engine(engine), m_stack(stack) {}

void Layer::popSelf() {
    m_stack->queueCommand([this]() { m_stack->popLayer(); });
}

}  // namespace opticrafter