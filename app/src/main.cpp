#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <opticrafter/OptiCrafter.h>

#include "MainMenuLayer.h"

int main() {
    std::unique_ptr<opticrafter::Engine> engine;

    try {
        engine = std::make_unique<opticrafter::Engine>();
        opticrafter::Logger::setAppLoggerLevel(opticrafter::LogLevel::TRACE);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForOpenGL(engine->window()->handle(), engine->window()->context());
        ImGui_ImplOpenGL3_Init();

        engine->window()->setVsync(1);

        engine->pushLayer<MainMenuLayer>();
        engine->run();
    } catch (const std::exception& e) {
        LOG_ERROR("Exception throwed: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
