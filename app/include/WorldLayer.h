#pragma once

#include "World.h"
#include "opticrafter/OptiCrafter.h"

class WorldLayer : public opticrafter::Layer {
   public:
    WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer);

    void onUpdate(float dt) override;
    void onRender() override;

   private:
    World m_world;
    opticrafter::Renderer& m_renderer;
    std::unique_ptr<opticrafter::Camera> m_camera;
};