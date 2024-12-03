#define APPLICATION_ENTRY_POINT
#include <Engine.hpp>

class Block;

class Minecraft : public Engine::Application {
   public:
    Minecraft();
    ~Minecraft() = default;

   private:
    Block* m_Block;
};

Engine::Application* Engine::CreateApplication() { return new Minecraft(); }