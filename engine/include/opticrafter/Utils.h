#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "Logger.h"

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

inline std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_CORE_ERROR("Cannot open shader file: {}", path.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}  // namespace opticrafter