#pragma once

#include "opticrafter/OptiCrafter.h"

class DebugLayer : public opticrafter::Layer {
   public:
    static constexpr float REFRESH_RATE_SECONDS = 0.5f;

    DebugLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine);

    void onEvent(SDL_Event& event) override;
    void onUpdate(float dt) override;
    void onRender() override;

   private:
    float m_accumulator = 0.f;
    float m_fps = 0.f;
};