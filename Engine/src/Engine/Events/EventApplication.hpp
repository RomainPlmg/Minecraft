#pragma once

#include "Event.hpp"

namespace Engine {

class ApplicationEvent : public Event {
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowResizeEvent final : public ApplicationEvent {
   public:
    int width, height;

    WindowResizeEvent(int width, int height) : width(width), height(height) {}

    EVENT_CLASS_TYPE(EventType::WindowResize)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Window resized to: " << width << "x" << height;
        return ss.str();
    }
};

class WindowCloseEvent final : public ApplicationEvent {
   public:
    EVENT_CLASS_TYPE(EventType::WindowClose)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Window closed";
        return ss.str();
    }
};

class WindowMovedEvent final : public ApplicationEvent {
   public:
    int xpos, ypos;

    WindowMovedEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}

    EVENT_CLASS_TYPE(EventType::WindowMoved)

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Window moved at: x=" << xpos << " | y=" << ypos;
        return ss.str();
    }
};

}  // namespace Engine