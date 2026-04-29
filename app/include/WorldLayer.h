#pragma once

#include "opticrafter/OptiCrafter.h"

class WorldLayer : public opticrafter::Layer {
   public:
    WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer);

    void onUpdate(float dt) override;
    void onRender() override;

   private:
    std::unique_ptr<opticrafter::Mesh> m_cube_mesh;
    opticrafter::Renderer& m_renderer;
};