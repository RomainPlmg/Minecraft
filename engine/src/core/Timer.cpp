#include "opticrafter/Timer.h"

#include <SDL3/SDL.h>

#include <cassert>

namespace opticrafter {

void Timer::reset() {
    m_dt = 0.f;
    m_last_time = SDL_GetPerformanceCounter();
}

void Timer::update() {
    uint64_t now = SDL_GetPerformanceCounter();
    m_dt = (float)(now - m_last_time) / (float)SDL_GetPerformanceFrequency();
    m_last_time = now;
}

}  // namespace opticrafter