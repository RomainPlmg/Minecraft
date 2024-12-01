#pragma once

#include "Engine/Core/Window.hpp"

struct GLFWwindow;

namespace Engine {

class LinuxWindow : public Window {
   public:
    LinuxWindow(const WindowProps& props);
    ~LinuxWindow() = default;

    virtual void Update() override;
    virtual void Close() override;
    virtual int ShouldClose() override;
    virtual void Shutdown() override;

    /* Getters */
    virtual inline void* GetHandler() const override { return m_Handler; }

    /* Setters */
    virtual void SetVSync(bool enable) override;

   private:
    GLFWwindow* m_Handler;

    /* Callback functions */
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
};

}  // namespace Engine