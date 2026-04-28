#pragma once

#include <SDL3/SDL.h>

namespace opticrafter {

class Input {
   public:
    static bool isKeyPressed(SDL_Keycode keycode);
    static void getMousePosition(float* x, float* y);
    static bool isMouseButtonPressed(SDL_MouseButtonFlags button);
};

}  // namespace opticrafter