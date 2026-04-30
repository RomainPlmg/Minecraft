#pragma once

#include "MeshBuilder.h"
#include "opticrafter/OptiCrafter.h"

class WorldLayer : public opticrafter::Layer {
   public:
    WorldLayer(opticrafter::LayerStack* stack, opticrafter::Renderer& renderer);

    void onUpdate(float dt) override;
    void onRender() override;

   private:
    MeshBuilder m_mesh_builder;
    std::unique_ptr<opticrafter::Camera> m_camera;
    opticrafter::TextureAtlas m_atlas;
    opticrafter::Renderer& m_renderer;
};