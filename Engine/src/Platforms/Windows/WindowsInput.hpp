#pragma once

#include "Engine/Core/Input.h"

namespace Engine {

class WindowsInput : public Input {
   protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) override;
    virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) override;
    virtual glm::vec2 GetMousePositionImpl() override;
};

}  // namespace Engine