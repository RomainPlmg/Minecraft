#include "EventDispatcher.hpp"

namespace Engine {

EventDispatcher::~EventDispatcher() { m_Listeners.clear(); }

void EventDispatcher::Subscribe(EventCategory category, const Listener& listener) {
    m_Listeners[category].push_back(listener);
}

void EventDispatcher::Unsubscribe(EventCategory category, const Listener& listener) {
    auto& listeners = m_Listeners[category];
    for (auto it = listeners.begin(); it != listeners.end(); ++it) {
        if (it->target_type() == listener.target_type() &&
            it->target<void()>() == listener.target<void>()) {
            listeners.erase(it);
            break;
        }
    }
}

void EventDispatcher::Dispatch(Event& event) {
    for (const auto& listeners : m_Listeners) {
        if (event.IsInCategory(listeners.first)) {
            for (const auto& listener : listeners.second) {
                listener(event);
            }
        }
    }
}

}  // namespace Engine