#include "DebugLayer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

DebugLayer::DebugLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine)
    : opticrafter::Layer(stack, engine) {}

void DebugLayer::onEvent(SDL_Event& event) { ImGui_ImplSDL3_ProcessEvent(&event); }

void DebugLayer::onUpdate(float dt) {
    static float smoothedFPS = 0.f;

    if (dt > 0.f) {
        m_accumulator += dt;
        float currentFPS = 1.f / dt;
        smoothedFPS = (smoothedFPS * 0.9f) + (currentFPS * 0.1f);
    }

    if (m_accumulator >= REFRESH_RATE_SECONDS) {
        m_accumulator -= REFRESH_RATE_SECONDS;
        m_fps = smoothedFPS;
    }
}

void DebugLayer::onRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %d", (int)m_fps);
    ImGui::Text("Draw calls: %d", m_engine.renderer()->stats().draw_calls);
    ImGui::Text("Triangles: %d", m_engine.renderer()->stats().triangles);
    ImGui::Text("Viewport resolution: %dx%d", m_engine.renderer()->viewport().w, m_engine.renderer()->viewport().h);
    ImGui::Text("Camera pos: %f/%f/%f", m_engine.renderer()->stats().camera_pos.x,
                m_engine.renderer()->stats().camera_pos.y, m_engine.renderer()->stats().camera_pos.z);

    static bool wireframe = false;
    if (ImGui::Checkbox("Wireframe", &wireframe)) {
        m_engine.renderer()->setWireframe(wireframe);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}