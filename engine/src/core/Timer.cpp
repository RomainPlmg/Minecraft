#include "opticrafter/Timer.h"

#include <SDL3/SDL.h>

#include <cassert>

namespace opticrafter {

void Timer::reset() {
    m_dt = 0.0;
    m_last_time = 0;
}

void Timer::update() {
    uint64_t now = SDL_GetPerformanceCounter();

    if (now - m_last_time != 0) {
        m_dt = (float)(now - m_last_time) / (float)SDL_GetPerformanceFrequency();
    }
    m_last_time = now;
}

}  // namespace opticrafter