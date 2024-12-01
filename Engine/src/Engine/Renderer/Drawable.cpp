#include "Drawable.hpp"

namespace Engine {

std::vector<Drawable*> Drawable::m_DrawableArray;

Drawable::Drawable() : m_Texture(nullptr), m_Shader(nullptr) { m_DrawableArray.push_back(this); }

}  // namespace Engine