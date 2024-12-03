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
    : m_RendererAPI(nullptr), m_ProjMatrix(glm::mat4(1.0f)), m_BufferLayout(nullptr) {}

Renderer::~Renderer() {
    delete m_RendererAPI;
    delete m_BufferLayout;
}

void Renderer::Init() {
    Application::GetInstance()->GetWindow()->GetEventDispatcher()->Subscribe(
        EventCategoryApplication, BIND_EVENT_FN(OnEvent));

    m_RendererAPI = RendererAPI::Create();
    m_RendererAPI->Init();

    m_BufferLayout = new BufferLayout({BufferElement(ShaderDataType::Float3, "position"),
                                       BufferElement(ShaderDataType::Float3, "color"),
                                       BufferElement(ShaderDataType::Float2, "uv")});

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
    if (Drawable::GetArray().empty()) return;

    Application::GetInstance()->GetTextureAtlas("blockAtlas")->Bind();

    for (const auto& shader : ShaderLibrary::GetShaderArray()) {
        shader->Bind();
        shader->SetUniformMat4("projMatrix", m_ProjMatrix);

        std::vector<float> vertices;

        for (const auto& object : Drawable::GetArray()) {
            if (object->GetShader() == shader) {
                vertices.insert(vertices.end(), object->GetVertices().begin(),
                                object->GetVertices().end());
            }
        }

        VertexBuffer* vbo = VertexBuffer::Create(vertices.data(),
                                                 static_cast<int>(vertices.size() * sizeof(float)));
        ElementBuffer* ebo = ElementBuffer::Create(m_Indices[shader].data(),
                                                   static_cast<uint32_t>(m_Indices[shader].size()));

        vbo->SetLayout(*m_BufferLayout);

        VertexArray* vao = VertexArray::Create();
        vao->AddVertexBuffer(vbo);
        vao->UpdateElementBuffer(ebo);

        vao->Bind();
        ebo->Bind();
        m_RendererAPI->Draw(ebo->GetCount());

        delete vbo;
        delete ebo;
        delete vao;
    }
}

void Renderer::IncreaseIndexArray(Shader* shader) {
    unsigned int baseIndex = static_cast<uint32_t>(m_Indices[shader].size() / 6 *
                                                   4);  // The last used index, based on vertices

    // Add indices for a new rectangle (2 triangles)
    m_Indices[shader].push_back(baseIndex + 0);  // Triangle 1 : first vertex
    m_Indices[shader].push_back(baseIndex + 1);  // Triangle 1 : second vertex
    m_Indices[shader].push_back(baseIndex + 3);  // Triangle 1 : third vertex
    m_Indices[shader].push_back(baseIndex + 1);  // Triangle 2 : first vertex
    m_Indices[shader].push_back(baseIndex + 2);  // Triangle 2 : second vertex
    m_Indices[shader].push_back(baseIndex + 3);  // Triangle 2 : third vertex
}

void Renderer::DecreaseIndexArray(Shader* shader) {
    if (m_Indices[shader].size() >= 6) {
        // Delete the last 6 indices
        m_Indices[shader].erase(m_Indices[shader].end() - 6, m_Indices[shader].end());
    }
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