#pragma once

#include <cstdint>

namespace opticrafter {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Viewport {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

}  // namespace opticrafter