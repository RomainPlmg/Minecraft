#include "opticrafter/Engine.h"

#include "opticrafter/Logger.h"

namespace opticrafter {

Engine::Engine() {
    Logger::init();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(SDL_GetError());
    }

    m_window = std::make_unique<Window>();

    LOG_CORE_DEBUG("Engine init successful!");
}

Engine::~Engine() {
    m_window.reset();
    SDL_Quit();
    LOG_CORE_DEBUG("Engine destroy successful!");
    LOG_CORE_INFO("Bye!");
}

void Engine::run() {
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        m_window->swapBuffers();
    }
}

}  // namespace opticrafter