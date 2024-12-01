#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.hpp"
#include "Drawable.hpp"
#include "Engine/Core/Application.hpp"
#include "Engine/Events/EventApplication.hpp"
#include "Engine/Utils/Color.hpp"
#include "RendererAPI.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "VertexArray.hpp"

namespace Engine {

Renderer::Renderer()
    : m_RendererAPI(nullptr), m_ProjMatrix(glm::mat4(1.0f)), m_TextureAtlas(nullptr) {}

Renderer::~Renderer() {
    delete m_RendererAPI;
    delete m_BufferLayout;
    delete m_TextureAtlas;
}

void Renderer::Init() {
    Application::GetInstance()->GetWindow()->GetEventDispatcher()->Subscribe(
        EventCategoryApplication, BIND_EVENT_FN(OnEvent));

    m_RendererAPI = RendererAPI::Create();
    m_RendererAPI->Init();

    m_TextureAtlas = TextureAtlas::Create();
    m_TextureAtlas->Init();

    m_BufferLayout = new BufferLayout({BufferElement(ShaderDataType::Float3, "position"),
                                       BufferElement(ShaderDataType::Float3, "color"),
                                       BufferElement(ShaderDataType::Float2, "uv")});

    m_Indices.push_back(0);
    m_Indices.push_back(1);
    m_Indices.push_back(3);
    m_Indices.push_back(1);
    m_Indices.push_back(2);
    m_Indices.push_back(3);

    m_ProjMatrix = glm::perspective(glm::radians(45.0f),
                                    (float)Application::GetInstance()->GetWindow()->GetWidth() /
                                        (float)Application::GetInstance()->GetWindow()->GetHeight(),
                                    0.1f, 100.0f);
}

void Renderer::Shutdown() {}

void Renderer::SetViewport(uint32_t width, uint32_t height) {
    m_RendererAPI->SetViewport(width, height);
}

void Renderer::SetClearColor(const Color& color) { m_RendererAPI->SetClearColor(color); }

void Renderer::Clear() { m_RendererAPI->Clear(); }

void Renderer::Draw() {
    std::vector<float> vertices;

    for (const auto& object : Drawable::GetArray()) {
        vertices.insert(vertices.end(), object->GetVertices().begin(), object->GetVertices().end());
    }

    TextureInfo::UV uv = m_TextureAtlas->GetTexture("stone")->GetAtlasUV();
    // clang-format off
    float vertices1[] = {-0.5f, -0.5f, -5.0f,     0.0f, 0.0f, 0.0f,   uv.uMin, uv.vMin,
                        -0.5f,  0.5f, -5.0f,     0.0f, 0.0f, 0.0f,   uv.uMin, uv.vMax,
                         0.5f,  0.5f, -5.0f,     0.0f, 0.0f, 0.0f,   uv.uMax, uv.vMax,
                         0.5f, -0.5f, -5.0f,     0.0f, 0.0f, 0.0f,   uv.uMax, uv.vMin,
                        };
    // clang-format on

    // VertexBuffer* vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
    VertexBuffer* vbo = VertexBuffer::Create(vertices1, 32 * sizeof(float));
    ElementBuffer* ebo =
        ElementBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size()));

    vbo->SetLayout(*m_BufferLayout);

    VertexArray* vao = VertexArray::Create();
    vao->AddVertexBuffer(vbo);
    vao->UpdateElementBuffer(ebo);

    ShaderLibrary::GetShader("default")->Bind();
    ShaderLibrary::GetShader("default")->SetUniformMat4("projMatrix", m_ProjMatrix);

    m_RendererAPI->Draw(ebo->GetCount());

    delete vbo;
    delete ebo;
    delete vao;
}

void Renderer::OnEvent(const Event& event) {
    if (event.GetType() == EventType::WindowResize) {
        const WindowResizeEvent* wrEvent = dynamic_cast<const WindowResizeEvent*>(&event);
        SetViewport(wrEvent->width, wrEvent->height);

        float matrixWidth = wrEvent->width <= 0 ? 1.0f : static_cast<float>(wrEvent->width);
        float matrixHeight = wrEvent->height <= 0 ? 1.0f : static_cast<float>(wrEvent->height);
        m_ProjMatrix =
            glm::perspective(glm::radians(45.0f), matrixWidth / matrixHeight, 0.1f, 100.0f);
    }
}

Renderer* Renderer::Create() { return new Renderer(); }

}  // namespace Engine