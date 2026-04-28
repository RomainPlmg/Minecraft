#include "opticrafter/Input.h"

namespace opticrafter {

bool Input::isKeyPressed(SDL_Keycode keycode) {
    int numkeys;
    const bool* state = SDL_GetKeyboardState(&numkeys);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, NULL);

    if (state[scancode]) return true;
    return false;
}

void Input::getMousePosition(float* x, float* y) { SDL_GetMouseState(x, y); }

bool Input::isMouseButtonPressed(SDL_MouseButtonFlags button) {
    SDL_MouseButtonFlags flags = SDL_GetMouseState(NULL, NULL);
    return (flags & 1 << (button - 1)) != 0;
}

}  // namespace opticrafter