#pragma once

#include "Core.hpp"
#include "Window.hpp"

namespace Engine {

class Renderer;
class Event;
class TextureAtlas;

class ENGINE_API Application {
   public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(const Event& event);

    /* Getters */
    inline Window* GetWindow() const { return m_Window; }
    inline Renderer* GetRenderer() const { return m_Renderer; }
    inline TextureAtlas* GetTextureAtlas(const std::string& name) const {
        return m_TextureAtlases.at(name);
    }

    static inline Application* GetInstance() { return m_Instance; }

   private:
    Window* m_Window;
    Renderer* m_Renderer;

   protected:
    std::unordered_map<std::string, TextureAtlas*> m_TextureAtlases;

    static Application* m_Instance;
};

// To be defined by the client
Application* CreateApplication();

}  // namespace Engine