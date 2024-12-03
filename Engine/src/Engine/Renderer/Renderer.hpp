#pragma once

#include <glm/glm.hpp>

namespace Engine {

class RendererAPI;
class Color;
class BufferLayout;
class TextureAtlas;
class Event;
class Shader;

class Renderer {
   public:
    Renderer();
    ~Renderer();

    void Init();
    void Shutdown();
    void SetViewport(uint32_t width, uint32_t height);
    void SetClearColor(const Color& color);
    void Clear();
    void Draw();
    void IncreaseIndexArray(Shader* shader);
    void DecreaseIndexArray(Shader* shader);

    void OnEvent(const Event& event);

    static Renderer* Create();

   private:
    RendererAPI* m_RendererAPI;

    BufferLayout* m_BufferLayout;
    std::unordered_map<Shader*, std::vector<uint32_t>> m_Indices;

    glm::mat4 m_ProjMatrix;
};

}  // namespace Engine