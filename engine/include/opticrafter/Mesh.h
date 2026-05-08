#pragma once

#include "Buffer.h"
#include "VertexArray.h"

namespace opticrafter {

struct Mesh {
    Mesh(std::span<const std::byte> vertices, std::span<const VertexAttrib> attributes, size_t stride,
         std::span<const uint32_t> indices)
        : vbo(vertices), ebo(indices) {
        vao.setLayout(vbo, ebo, attributes, stride);
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    VertexBuffer vbo;
    IndexBuffer ebo;
    VertexArray vao;
};

}  // namespace opticrafter