#pragma once

#include "World.h"
#include "opticrafter/OptiCrafter.h"

class WorldLayer : public opticrafter::Layer {
   public:
    WorldLayer(opticrafter::LayerStack* stack, opticrafter::Engine& engine);

    void onEvent(SDL_Event& event) override;
    void onUpdate(float dt) override;
    void onRender() override;

   private:
    World m_world;
    std::unique_ptr<opticrafter::Camera> m_camera;
};