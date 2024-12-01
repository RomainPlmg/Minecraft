#pragma once

namespace Engine {

/* From glfw3.h*/
enum MouseButtonCode {
    MouseButton1 = 0,
    MouseButton2 = 1,
    MouseButton3 = 2,
    MouseButton4 = 3,
    MouseButton5 = 4,
    MouseButton6 = 5,
    MouseButton7 = 6,
    MouseButton8 = 7,
    MouseButtonLast = MouseButton8,
    MouseButtonLeft = MouseButton1,
    MouseButtonRight = MouseButton2,
    MouseButtonMiddle = MouseButton3
};

const std::map<MouseButtonCode, std::string> MouseButtonNames{
    std::make_pair(MouseButton1, "MouseButtonLeft"),
    std::make_pair(MouseButton2, "MouseButtonRight"),
    std::make_pair(MouseButton3, "MouseButtonMiddle"),
    std::make_pair(MouseButton4, "MouseButton4"),
    std::make_pair(MouseButton5, "MouseButton5"),
    std::make_pair(MouseButton6, "MouseButton6"),
    std::make_pair(MouseButton7, "MouseButton7"),
    std::make_pair(MouseButton8, "MouseButton8"),
    std::make_pair(MouseButtonLast, "MouseButtonLast"),
    std::make_pair(MouseButtonLeft, "MouseButtonLeft"),
    std::make_pair(MouseButtonRight, "MouseButtonRight"),
    std::make_pair(MouseButtonMiddle, "MouseButtonMiddle")};

}  // namespace Engine