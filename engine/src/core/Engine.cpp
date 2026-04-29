#include "opticrafter/Engine.h"

#include "opticrafter/Layer.h"
#include "opticrafter/Logger.h"
#include "opticrafter/Timer.h"

namespace opticrafter {

Engine::Engine() {
    Logger::init();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = std::make_unique<Window>();
    m_renderer = std::make_unique<Renderer>(*m_window->context());

    LOG_CORE_INFO("Engine init successful!");
}

Engine::~Engine() {
    m_renderer.reset();
    m_window.reset();
    SDL_Quit();
    LOG_CORE_DEBUG("Engine destroy successful!");
    LOG_CORE_INFO("Bye!");
}

void Engine::run() {
    bool running = true;
    m_timer.reset();

    while (running) {
        m_timer.update();
        auto dt = m_timer.dt();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }
        m_event_bus.pollEvents();

        m_renderer->clear();

        for (const auto& layer : m_layer_stack) {
            layer->onUpdate(dt);
        }

        for (const auto& layer : m_layer_stack) {
            layer->onRender();
        }

        m_layer_stack.processCommands();

        m_window->swapBuffers();
    }
}

}  // namespace opticrafter