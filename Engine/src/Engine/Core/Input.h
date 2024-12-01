#pragma once

#include <glm/glm.hpp>

#include "Application.hpp"
#include "Core.hpp"
#include "Engine/Utils/KeyCodes.hpp"
#include "Engine/Utils/MouseButtonCodes.hpp"

namespace Engine {

class ENGINE_API Input {
   public:
    static inline bool IsKeyPressed(KeyCode keycode) {
        return m_Instance->IsKeyPressedImpl(keycode);
    }
    static inline bool IsMouseButtonPressed(MouseButtonCode button) {
        return m_Instance->IsMouseButtonPressedImpl(button);
    }
    static inline glm::vec2 GetMousePosition() { return m_Instance->GetMousePositionImpl(); }

   private:
    static Input* m_Instance;

   protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
    virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
    virtual glm::vec2 GetMousePositionImpl() = 0;
};

}  // namespace Engine
