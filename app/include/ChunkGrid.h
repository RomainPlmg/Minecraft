#pragma once

#include "Chunk.h"

class ChunkGrid {
   public:
    ChunkGrid(uint8_t render_distance, int ox = 0, int oz = 0);

    Chunk* getChunk(int cx, int cz) const;
    bool isInBounds(int cx, int cz) const;

    void setOrigin(int ox, int oz);

    auto begin() const { return m_chunks.begin(); }
    auto end() const { return m_chunks.end(); }

   private:
    int m_ox, m_oz;  // Origins
    int m_size = 0;
    opticrafter::RingBuffer2D<std::unique_ptr<Chunk>> m_chunks;
    std::vector<glm::ivec2> m_invalidated;
};