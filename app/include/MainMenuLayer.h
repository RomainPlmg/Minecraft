#pragma once

#include "opticrafter/OptiCrafter.h"

class MainMenuLayer : public opticrafter::Layer {
   public:
    MainMenuLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine) : opticrafter::Layer(stack, engine) {}

    void onEvent(SDL_Event& event) override;
    void onUpdate(float dt) override;
    void onRender() override;
};