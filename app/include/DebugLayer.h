#pragma once

#include "opticrafter/OptiCrafter.h"

class DebugLayer : public opticrafter::Layer {
   public:
    DebugLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine);

    void onEvent(SDL_Event& event) override;
    void onUpdate(float dt) override;
    void onRender() override;

   private:
    float m_fps = 0.f;
    opticrafter::Engine& m_engine;
};