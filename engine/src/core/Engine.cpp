#include "opticrafter/Engine.h"

#include <tracy/Tracy.hpp>

#include "opticrafter/Layer.h"
#include "opticrafter/Logger.h"
#include "opticrafter/Timer.h"

namespace opticrafter {

Engine::Engine() : m_thread_pool(std::thread::hardware_concurrency() - 4) {
    Logger::init();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
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
    m_timer.reset();

    while (m_running) {
        ZoneScopedN("MainLoop");
        m_timer.update();
        auto dt = m_timer.dt();

        {
            ZoneScopedN("PollEvents");
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) m_running = false;
                if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                    if (event.window.data1 >= 0 && event.window.data2 >= 0)
                        m_renderer->setViewport({0, 0, (uint32_t)event.window.data1, (uint32_t)event.window.data2});
                }
                for (const auto& layer : m_layer_stack) {
                    layer->onEvent(event);
                }
            }
            m_event_bus.pollEvents();
        }

        {
            ZoneScopedN("Clear");
            m_renderer->clear();
        }

        for (const auto& layer : m_layer_stack) {
            layer->onUpdate(dt);
        }

        for (const auto& layer : m_layer_stack) {
            layer->onRender();
        }

        {
            ZoneScopedN("ProcessCommands");
            m_layer_stack.processCommands();
        }

        {
            ZoneScopedN("SwapBuffers");
            m_window->swapBuffers();
        }

        FrameMark;
    }
}

void Engine::quit() { m_running = false; }

}  // namespace opticrafter