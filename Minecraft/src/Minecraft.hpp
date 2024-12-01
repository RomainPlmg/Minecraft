#include <Engine.hpp>

class Minecraft : public Engine::Application {
   public:
    Minecraft();

   private:
};

Engine::Application* Engine::CreateApplication() { return new Minecraft(); }