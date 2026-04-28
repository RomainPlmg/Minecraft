#include "opticrafter/LayerStack.h"

#include "opticrafter/Layer.h"

namespace opticrafter {

void LayerStack::pushLayer(std::unique_ptr<Layer> layer) { m_layers.push_back(std::move(layer)); }

void LayerStack::popLayer() {
    if (!m_layers.empty()) {
        m_layers.pop_back();
    }
}

void LayerStack::replaceTop(std::unique_ptr<Layer> layer) {
    if (!m_layers.empty()) {
        m_layers.pop_back();
    }
    m_layers.push_back(std::move(layer));
}

void LayerStack::queueCommand(Command command) { m_commands.push(std::move(command)); }

void LayerStack::processCommands() {
    while (!m_commands.empty()) {
        m_commands.front()();
        m_commands.pop();
    }
}

}  // namespace opticrafter