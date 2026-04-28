#include "opticrafter/EventBus.h"

namespace opticrafter {

void EventBus::pollEvents() {
    auto queue = std::move(m_queue);  // Copy and clear the queue
    m_queue.clear();

    for (auto& [typeIdx, event] : queue) {
        if (!m_callbacks.contains(typeIdx)) {
            LOG_CORE_WARN("Event {} emitted but no subscribers, skip...", typeIdx.name());
            continue;
        }
        for (auto& callback : m_callbacks[typeIdx]) {
            callback(event.get());
        }
    }
}

}  // namespace opticrafter