#include "MainMenuLayer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "WorldLayer.h"

void MainMenuLayer::onEvent(SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

void MainMenuLayer::onUpdate(float dt) {}

void MainMenuLayer::onRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                    ImGuiWindowFlags_NoNav;

    // Center window
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    window_flags |= ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("Example: Simple overlay", nullptr, window_flags)) {
        if (ImGui::Button("Play")) {
            transitionTo<WorldLayer>();
        }
        if (ImGui::Button("Quit")) {
            m_engine.quit();
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}