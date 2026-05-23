#include "GreedyMesher.h"

#include <tracy/Tracy.hpp>

static int vertexAO(bool side1, bool side2, bool corner) {
    if (side1 && side2) return 3;
    return side1 + side2 + corner;
}

void GreedyMesher::run(const std::span<BlockType>& grid, MeshBuilder& builder) {
    xPass(grid, builder);
    yPass(grid, builder);
    zPass(grid, builder);
}

void GreedyMesher::xPass(const std::span<BlockType>& grid, MeshBuilder& builder) {
    for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
        // Init mask
        for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
                auto block = grid[index(x + 1, y, z + 1)];
                int mask_idx = y + z * Chunk::CHUNK_HEIGHT;

                // === RIGHT (Face +X) ===
                m_right[mask_idx].visited = false;
                m_right[mask_idx].type = block;
                m_right[mask_idx].pos = glm::ivec3(x, y, z);
                m_right[mask_idx].visible = block != BlockType::AIR;

                auto right_neighbor = grid[index(x + 2, y, z + 1)];
                m_right[mask_idx].visible &=
                    ((right_neighbor == BlockType::AIR) || m_registry.get(right_neighbor).transparent);
                if (m_right[mask_idx].visible) {
                    m_right[mask_idx].ao = computeAO(grid, m_right[mask_idx].pos, {1, 0, 0}, {0, 0, 1}, {0, 1, 0});
                }

                // === LEFT (Face -X) ===
                m_left[mask_idx].visited = false;
                m_left[mask_idx].type = block;
                m_left[mask_idx].pos = glm::ivec3(x, y, z);
                m_left[mask_idx].visible = block != BlockType::AIR;

                // X est paddé dans ton indexeur, donc `x` local pointe vers la bordure de gauche en mémoire
                auto left_neighbor = grid[index(x, y, z + 1)];
                m_left[mask_idx].visible &=
                    ((left_neighbor == BlockType::AIR) || m_registry.get(left_neighbor).transparent);
                if (m_left[mask_idx].visible) {
                    m_left[mask_idx].ao = computeAO(grid, m_left[mask_idx].pos, {-1, 0, 0}, {0, 0, 1}, {0, 1, 0});
                }
            }
        }

        // Run algo RIGHT
        for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
                int curr_idx = y + z * Chunk::CHUNK_HEIGHT;

                if (!m_right[curr_idx].visited && m_right[curr_idx].visible) {
                    uint8_t span_y = 1;
                    uint8_t span_z = 1;

                    // Extrusion Y
                    int next_y = y + 1;
                    while (next_y < Chunk::CHUNK_HEIGHT) {
                        int next_idx = next_y + z * Chunk::CHUNK_HEIGHT;
                        if (!m_right[next_idx].visible || m_right[next_idx].visited ||
                            m_right[curr_idx].type != m_right[next_idx].type ||
                            m_right[curr_idx].ao != m_right[next_idx].ao) {
                            break;
                        }
                        span_y++;
                        next_y++;
                    }

                    // Extrusion Z
                    int next_z = z + 1;
                    bool row_valid = true;
                    while (next_z < Chunk::CHUNK_WIDTH && row_valid) {
                        for (int w = 0; w < span_y; w++) {
                            int next_idx = (y + w) + next_z * Chunk::CHUNK_HEIGHT;
                            if (!m_right[next_idx].visible || m_right[next_idx].visited ||
                                m_right[curr_idx].type != m_right[next_idx].type ||
                                m_right[curr_idx].ao != m_right[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            span_z++;
                            next_z++;
                        }
                    }

                    auto& block_def = m_registry.get(m_right[curr_idx].type);
                    // L'ORDRE EST ICI VITAL : {Z, Y} pour coller avec tes vecteurs {t, bt}
                    builder.addRightFace(m_right[curr_idx].pos, {span_z, span_y}, block_def.side, m_right[curr_idx].ao);

                    // Verrouillage du masque
                    for (int hz = 0; hz < span_z; hz++) {
                        for (int hy = 0; hy < span_y; hy++) {
                            int lock_idx = (y + hy) + (z + hz) * Chunk::CHUNK_HEIGHT;
                            m_right[lock_idx].visited = true;
                        }
                    }
                }

                // Run algo LEFT
                if (!m_left[curr_idx].visited && m_left[curr_idx].visible) {
                    uint8_t span_y = 1;
                    uint8_t span_z = 1;

                    int next_y = y + 1;
                    while (next_y < Chunk::CHUNK_HEIGHT) {
                        int next_idx = next_y + z * Chunk::CHUNK_HEIGHT;
                        if (!m_left[next_idx].visible || m_left[next_idx].visited ||
                            m_left[curr_idx].type != m_left[next_idx].type ||
                            m_left[curr_idx].ao != m_left[next_idx].ao) {
                            break;
                        }
                        span_y++;
                        next_y++;
                    }

                    int next_z = z + 1;
                    bool row_valid = true;
                    while (next_z < Chunk::CHUNK_WIDTH && row_valid) {
                        for (int w = 0; w < span_y; w++) {
                            int next_idx = (y + w) + next_z * Chunk::CHUNK_HEIGHT;
                            if (!m_left[next_idx].visible || m_left[next_idx].visited ||
                                m_left[curr_idx].type != m_left[next_idx].type ||
                                m_left[curr_idx].ao != m_left[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            span_z++;
                            next_z++;
                        }
                    }

                    auto& block_def = m_registry.get(m_left[curr_idx].type);
                    builder.addLeftFace(m_left[curr_idx].pos, {span_z, span_y}, block_def.side, m_left[curr_idx].ao);

                    for (int hz = 0; hz < span_z; hz++) {
                        for (int hy = 0; hy < span_y; hy++) {
                            int lock_idx = (y + hy) + (z + hz) * Chunk::CHUNK_HEIGHT;
                            m_left[lock_idx].visited = true;
                        }
                    }
                }
            }
        }
    }
}

void GreedyMesher::yPass(const std::span<BlockType>& grid, MeshBuilder& builder) {
    for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
        // Init mask
        for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block = grid[index(x + 1, y, z + 1)];
                int mask_idx = x + z * Chunk::CHUNK_WIDTH;

                // === TOP ===
                m_top[mask_idx].visited = false;
                m_top[mask_idx].type = block;
                m_top[mask_idx].pos = glm::ivec3(x, y, z);
                m_top[mask_idx].visible = block != BlockType::AIR;
                if (y < Chunk::CHUNK_HEIGHT - 1) {  // Skip top cubes, top faces are rendered anyway...
                    auto neighbor = grid[index(x + 1, y + 1, z + 1)];
                    m_top[mask_idx].visible &= ((neighbor == BlockType::AIR) || m_registry.get(neighbor).transparent);
                }

                if (m_top[mask_idx].visible) {
                    m_top[mask_idx].ao = computeAO(grid, m_top[mask_idx].pos, {0, 1, 0}, {1, 0, 0}, {0, 0, 1});
                }

                // === BOTTOM ===
                m_bottom[mask_idx].visited = false;
                m_bottom[mask_idx].type = block;
                m_bottom[mask_idx].pos = glm::ivec3(x, y, z);
                m_bottom[mask_idx].visible = block != BlockType::AIR;
                if (y > 0) {  // Skip floor cubes, bottom faces are rendered anyway...
                    auto neighbor = grid[index(x + 1, y - 1, z + 1)];
                    m_bottom[mask_idx].visible &=
                        ((neighbor == BlockType::AIR) || m_registry.get(neighbor).transparent);
                }

                if (m_bottom[mask_idx].visible) {
                    m_bottom[mask_idx].ao = computeAO(grid, m_bottom[mask_idx].pos, {0, -1, 0}, {1, 0, 0}, {0, 0, 1});
                }
            }
        }

        // Run algo
        for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                int curr_idx = x + z * Chunk::CHUNK_WIDTH;

                if (!m_top[curr_idx].visited && m_top[curr_idx].visible) {
                    uint8_t width = 1;
                    uint8_t height = 1;

                    // Look on the X axis until non-mergable block
                    int next_x = x + 1;
                    while (next_x < Chunk::CHUNK_WIDTH) {
                        int next_idx = next_x + z * Chunk::CHUNK_WIDTH;
                        if (!m_top[next_idx].visible || m_top[next_idx].visited ||
                            m_top[curr_idx].type != m_top[curr_idx].type || m_top[curr_idx].ao != m_top[next_idx].ao) {
                            break;
                        }
                        width++;
                        next_x++;
                    }

                    int next_z = z + 1;
                    bool row_valid = true;
                    while (next_z < Chunk::CHUNK_WIDTH && row_valid) {
                        for (int w = 0; w < width; w++) {
                            int next_idx = x + w + next_z * Chunk::CHUNK_WIDTH;
                            if (!m_top[next_idx].visible || m_top[next_idx].visited ||
                                m_top[curr_idx].type != m_top[next_idx].type ||
                                m_top[curr_idx].ao != m_top[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            height++;
                            next_z++;
                        }
                    }

                    auto& block_def = m_registry.get(m_top[curr_idx].type);
                    builder.addTopFace(m_top[curr_idx].pos, {width, height}, block_def.top, m_top[curr_idx].ao);

                    for (int hz = 0; hz < height; hz++) {
                        for (int hw = 0; hw < width; hw++) {
                            int lock_idx = (x + hw) + (z + hz) * Chunk::CHUNK_WIDTH;
                            m_top[lock_idx].visited = true;
                        }
                    }
                }

                if (!m_bottom[curr_idx].visited && m_bottom[curr_idx].visible) {
                    uint8_t width = 1;
                    uint8_t height = 1;

                    // Look on the X axis until non-mergable block
                    int next_x = x + 1;
                    while (next_x < Chunk::CHUNK_WIDTH) {
                        int next_idx = next_x + z * Chunk::CHUNK_WIDTH;
                        if (!m_bottom[next_idx].visible || m_bottom[next_idx].visited ||
                            m_bottom[curr_idx].type != m_bottom[curr_idx].type ||
                            m_bottom[curr_idx].ao != m_bottom[next_idx].ao) {
                            break;
                        }
                        width++;
                        next_x++;
                    }

                    int next_z = z + 1;
                    bool row_valid = true;
                    while (next_z < Chunk::CHUNK_WIDTH && row_valid) {
                        for (int w = 0; w < width; w++) {
                            int next_idx = x + w + next_z * Chunk::CHUNK_WIDTH;
                            if (!m_bottom[next_idx].visible || m_bottom[next_idx].visited ||
                                m_bottom[curr_idx].type != m_bottom[next_idx].type ||
                                m_bottom[curr_idx].ao != m_bottom[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            height++;
                            next_z++;
                        }
                    }

                    auto& block_def = m_registry.get(m_bottom[curr_idx].type);
                    builder.addBottomFace(m_bottom[curr_idx].pos, {width, height}, block_def.bottom,
                                          m_bottom[curr_idx].ao);

                    for (int hz = 0; hz < height; hz++) {
                        for (int hw = 0; hw < width; hw++) {
                            int lock_idx = (x + hw) + (z + hz) * Chunk::CHUNK_WIDTH;
                            m_bottom[lock_idx].visited = true;
                        }
                    }
                }
            }
        }
    }
}

void GreedyMesher::zPass(const std::span<BlockType>& grid, MeshBuilder& builder) {
    for (int z = 0; z < Chunk::CHUNK_WIDTH; z++) {
        // Init mask
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                auto block = grid[index(x + 1, y, z + 1)];
                int mask_idx = x + y * Chunk::CHUNK_WIDTH;

                // === FRONT (Face +Z) ===
                m_front[mask_idx].visited = false;
                m_front[mask_idx].type = block;
                m_front[mask_idx].pos = glm::ivec3(x, y, z);
                m_front[mask_idx].visible = block != BlockType::AIR;

                auto front_neighbor = grid[index(x + 1, y, z + 2)];
                m_front[mask_idx].visible &=
                    ((front_neighbor == BlockType::AIR) || m_registry.get(front_neighbor).transparent);
                if (m_front[mask_idx].visible) {
                    m_front[mask_idx].ao = computeAO(grid, m_front[mask_idx].pos, {0, 0, 1}, {1, 0, 0}, {0, 1, 0});
                }

                // === BACK (Face -Z) ===
                m_back[mask_idx].visited = false;
                m_back[mask_idx].type = block;
                m_back[mask_idx].pos = glm::ivec3(x, y, z);
                m_back[mask_idx].visible = block != BlockType::AIR;

                auto back_neighbor = grid[index(x + 1, y, z)];
                m_back[mask_idx].visible &=
                    ((back_neighbor == BlockType::AIR) || m_registry.get(back_neighbor).transparent);
                if (m_back[mask_idx].visible) {
                    m_back[mask_idx].ao = computeAO(grid, m_back[mask_idx].pos, {0, 0, -1}, {1, 0, 0}, {0, 1, 0});
                }
            }
        }

        // Run algo
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < Chunk::CHUNK_WIDTH; x++) {
                int curr_idx = x + y * Chunk::CHUNK_WIDTH;

                // ---- RUN FRONT ----
                if (!m_front[curr_idx].visited && m_front[curr_idx].visible) {
                    uint8_t span_x = 1;
                    uint8_t span_y = 1;

                    // Extrusion sur l'axe X
                    int next_x = x + 1;
                    while (next_x < Chunk::CHUNK_WIDTH) {
                        int next_idx = next_x + y * Chunk::CHUNK_WIDTH;
                        if (!m_front[next_idx].visible || m_front[next_idx].visited ||
                            m_front[curr_idx].type != m_front[next_idx].type ||
                            m_front[curr_idx].ao != m_front[next_idx].ao) {
                            break;
                        }
                        span_x++;
                        next_x++;
                    }

                    // Extrusion sur l'axe Y
                    int next_y = y + 1;
                    bool row_valid = true;
                    while (next_y < Chunk::CHUNK_HEIGHT && row_valid) {
                        for (int w = 0; w < span_x; w++) {
                            int next_idx = (x + w) + next_y * Chunk::CHUNK_WIDTH;
                            if (!m_front[next_idx].visible || m_front[next_idx].visited ||
                                m_front[curr_idx].type != m_front[next_idx].type ||
                                m_front[curr_idx].ao != m_front[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            span_y++;
                            next_y++;
                        }
                    }

                    auto& block_def = m_registry.get(m_front[curr_idx].type);
                    // L'ordre passé au builder correspond à l'axe {X, Y} du plan
                    builder.addFrontFace(m_front[curr_idx].pos, {span_x, span_y}, block_def.side, m_front[curr_idx].ao);

                    for (int hy = 0; hy < span_y; hy++) {
                        for (int hx = 0; hx < span_x; hx++) {
                            int lock_idx = (x + hx) + (y + hy) * Chunk::CHUNK_WIDTH;
                            m_front[lock_idx].visited = true;
                        }
                    }
                }

                // ---- RUN BACK ----
                if (!m_back[curr_idx].visited && m_back[curr_idx].visible) {
                    uint8_t span_x = 1;
                    uint8_t span_y = 1;

                    // Extrusion sur l'axe X
                    int next_x = x + 1;
                    while (next_x < Chunk::CHUNK_WIDTH) {
                        int next_idx = next_x + y * Chunk::CHUNK_WIDTH;
                        if (!m_back[next_idx].visible || m_back[next_idx].visited ||
                            m_back[curr_idx].type != m_back[next_idx].type ||
                            m_back[curr_idx].ao != m_back[next_idx].ao) {
                            break;
                        }
                        span_x++;
                        next_x++;
                    }

                    // Extrusion sur l'axe Y
                    int next_y = y + 1;
                    bool row_valid = true;
                    while (next_y < Chunk::CHUNK_HEIGHT && row_valid) {
                        for (int w = 0; w < span_x; w++) {
                            int next_idx = (x + w) + next_y * Chunk::CHUNK_WIDTH;
                            if (!m_back[next_idx].visible || m_back[next_idx].visited ||
                                m_back[curr_idx].type != m_back[next_idx].type ||
                                m_back[curr_idx].ao != m_back[next_idx].ao) {
                                row_valid = false;
                                break;
                            }
                        }
                        if (row_valid) {
                            span_y++;
                            next_y++;
                        }
                    }

                    auto& block_def = m_registry.get(m_back[curr_idx].type);
                    builder.addBackFace(m_back[curr_idx].pos, {span_x, span_y}, block_def.side, m_back[curr_idx].ao);

                    for (int hy = 0; hy < span_y; hy++) {
                        for (int hx = 0; hx < span_x; hx++) {
                            int lock_idx = (x + hx) + (y + hy) * Chunk::CHUNK_WIDTH;
                            m_back[lock_idx].visited = true;
                        }
                    }
                }
            }
        }
    }
}

std::array<int, 4> GreedyMesher::computeAO(const std::span<BlockType>& grid, const glm::ivec3& pos,
                                           const glm::ivec3 normal, const glm::ivec3& t, const glm::ivec3& bt) {
    ZoneScopedN("GreedyMesher::computeAO");
    std::array<int, 4> ao;

    int i = 0;
    for (auto& uv : {glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(1, 1), glm::ivec2(-1, 1)}) {
        glm::ivec3 s1_pos = pos + normal + t * uv.x;             // side1
        glm::ivec3 s2_pos = pos + normal + bt * uv.y;            // side2
        glm::ivec3 c_pos = pos + normal + t * uv.x + bt * uv.y;  // corner

        auto side1 = getBlockLocal(grid, s1_pos.x, s1_pos.y, s1_pos.z);
        auto side2 = getBlockLocal(grid, s2_pos.x, s2_pos.y, s2_pos.z);
        auto corner = getBlockLocal(grid, c_pos.x, c_pos.y, c_pos.z);

        bool bs1 = !m_registry.get(side1).transparent;
        bool bs2 = !m_registry.get(side2).transparent;
        bool bc = !m_registry.get(corner).transparent;

        ao[i] = vertexAO(bs1, bs2, bc);

        i++;
    }

    return ao;
}

inline size_t GreedyMesher::index(int x, int y, int z) const {
    return x + (Chunk::CHUNK_WIDTH + 2) * (y + Chunk::CHUNK_HEIGHT * z);
}

inline BlockType GreedyMesher::getBlockLocal(const std::span<BlockType>& grid, int x, int y, int z) {
    if (y < 0 || y >= Chunk::CHUNK_HEIGHT) return BlockType::AIR;
    return grid[index(x + 1, y, z + 1)];
}