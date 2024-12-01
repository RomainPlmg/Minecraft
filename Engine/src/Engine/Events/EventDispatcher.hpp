#pragma once

#include "Event.hpp"

namespace Engine {

class EventDispatcher {
   public:
    EventDispatcher() = default;
    ~EventDispatcher();

    using Listener = std::function<void(const Event&)>;

    // Add a listener to a specific event category
    void Subscribe(EventCategory category, const Listener& listener);
    // Delete a listener to a specific event category
    void Unsubscribe(EventCategory category, const Listener& listener);
    // Push an event on the bus
    void Dispatch(Event& event);

   private:
    std::unordered_map<EventCategory, std::vector<Listener>> m_Listeners;
};

}  // namespace Engine
