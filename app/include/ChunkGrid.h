#pragma once

#include "Chunk.h"

class ChunkGrid {
   public:
    ChunkGrid(uint8_t render_distance, glm::ivec2 origin = {});

    Chunk* getChunk(int cx, int cz) const;
    bool isInBounds(int cx, int cz) const;

    auto begin() const { return m_chunks.begin(); }
    auto end() const { return m_chunks.end(); }
    Chunk* operator[](size_t index) { return m_chunks[index].get(); }

   private:
    glm::ivec2 m_origin;
    int m_size = 0;                                // render_distance * 2 + 1
    std::vector<std::unique_ptr<Chunk>> m_chunks;  // m_size * m_size

    inline size_t index(int cx, int cz) const;
};