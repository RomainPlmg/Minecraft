#pragma once

#include "Core.hpp"
#include "Engine/Events/EventDispatcher.hpp"
#include "Engine/Renderer/GraphicContext.hpp"

namespace Engine {

struct WindowProps {
    std::string title;
    uint32_t width;
    uint32_t height;
    bool vsync;

    /* Default window props */
    WindowProps(std::string title = "Minecraft", uint32_t width = 1280, uint32_t height = 720);
};

// Virtual Window class, will be inherited by a platform specific window (Windows, Linux, etc.)
class ENGINE_API Window {
   public:
    Window();
    virtual ~Window();

    virtual void Update() = 0;
    virtual void Close() = 0;
    virtual int ShouldClose() = 0;
    virtual void Shutdown() = 0;

    virtual void OnEvent(const Event& event);

    /* Getters */
    inline uint32_t GetWidth() const { return m_Props.width; }
    inline uint32_t GetHeight() const { return m_Props.height; }
    inline bool IsVSync() const { return m_Props.vsync; }
    inline EventDispatcher* GetEventDispatcher() const { return m_EventDispatcher; }
    virtual void* GetHandler() const = 0;

    /* Setters */
    virtual void SetVSync(bool enable) = 0;

    /* Return pointer on the specific platform window */
    static Window* Create(const WindowProps& props = WindowProps());

   protected:
    WindowProps m_Props;
    GraphicContext* m_GraphicContext = nullptr;
    EventDispatcher* m_EventDispatcher = nullptr;
};

}  // namespace Engine