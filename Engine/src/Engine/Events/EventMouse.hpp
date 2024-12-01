#pragma once

#include "Engine/Utils/MouseButtonCodes.hpp"
#include "Event.hpp"

namespace Engine {

class MouseEvent : public Event {
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseMotionEvent : public MouseEvent {
   public:
    MouseMotionEvent(double x, double y) : posX(x), posY(y) {}

    double posX, posY;

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mouse moved at: x=" << posX << " | y=" << posY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseMoved)
};

class MouseButtonPressedEvent : public MouseEvent {
   public:
    MouseButtonPressedEvent(MouseButtonCode mouseButtonCode) : buttonCode(mouseButtonCode) {}

    MouseButtonCode buttonCode;

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressed: " << MouseButtonNames.at(buttonCode);
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseEvent {
   public:
    MouseButtonReleasedEvent(MouseButtonCode mouseButtonCode) : buttonCode(mouseButtonCode) {}

    MouseButtonCode buttonCode;

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleased: " << MouseButtonNames.at(buttonCode);
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
};

}  // namespace Engine