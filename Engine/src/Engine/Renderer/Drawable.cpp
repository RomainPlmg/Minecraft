#include "Drawable.hpp"

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Log.hpp"
#include "Renderer.hpp"

namespace Engine {

std::vector<Drawable*> Drawable::m_DrawableArray = {};

Drawable::~Drawable() {
    Unregister();
    CORE_TRACE("Drawable destroyed");
}

void Drawable::Register() {
    uint32_t nbRectangles = static_cast<uint32_t>(m_Vertices.size()) / 32;

    for (uint32_t i = 0; i < nbRectangles; i++) {
        Application::GetInstance()->GetRenderer()->IncreaseIndexArray(m_Shader);
    }

    m_DrawableArray.push_back(this);
}

void Drawable::Unregister() {
    auto it = std::find(m_DrawableArray.begin(), m_DrawableArray.end(), this);
    if (it != m_DrawableArray.end()) {
        m_DrawableArray.erase(it);  // Retire l'objet du tableau
    }
}

}  // namespace Engine