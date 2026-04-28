#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <vector>

namespace opticrafter {

class Layer;

class LayerStack {
   public:
    void pushLayer(std::unique_ptr<Layer> layer);
    void popLayer();
    void replaceTop(std::unique_ptr<Layer> layer);

    using Command = std::function<void()>;

    // Deferred commands (avoids changes during iteration)
    void queueCommand(Command command);
    void processCommands();

    // Iterations
    [[nodiscard]] auto begin() { return m_layers.begin(); }
    [[nodiscard]] auto end() { return m_layers.end(); }
    [[nodiscard]] auto rbegin() { return m_layers.rbegin(); }
    [[nodiscard]] auto rend() { return m_layers.rend(); }

   private:
    std::vector<std::unique_ptr<Layer>> m_layers;
    std::queue<Command> m_commands;
};

}  // namespace opticrafter