#pragma once

namespace Engine {

class Color {
   public:
    Color() : r(0.0f), g(0.0f), b(0.0f), a(255.0f){};
    Color(float r, float g, float b, float a = 255) : r(r), g(g), b(b), a(a){};
    ~Color() = default;

    /* Attributes */
    float r, g, b, a;

    Color ToGLColor() const { return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
};

inline const Color Color::Black = Color(0.0f, 0.0f, 0.0f);
inline const Color Color::White = Color(255.0f, 255.0f, 255.0f);
inline const Color Color::Red = Color(255.0f, 0.0f, 0.0f);
inline const Color Color::Green = Color(0.0f, 255.0f, 0.0f);
inline const Color Color::Blue = Color(0.0f, 0.0f, 255.0f);

}  // namespace Engine