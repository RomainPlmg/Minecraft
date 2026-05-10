#pragma once

#include <shared_mutex>

#include "Chunk.h"

class ChunkGrid {
   public:
    ChunkGrid(opticrafter::ThreadPool& thread_pool, uint8_t render_distance, int ox = 0, int oz = 0);

    Chunk* getChunk(int cx, int cz) const;
    bool isInBounds(int cx, int cz) const;

    void setOrigin(opticrafter::ThreadPool& thread_pool, int ox, int oz);

    auto begin() const { return m_chunks.begin(); }
    auto end() const { return m_chunks.end(); }

    std::vector<Chunk*> pollDirtyChunks(size_t max);
    std::vector<glm::ivec2> pollInvalidatedChunks();
    void pollPendingChunks();

   private:
    int m_ox, m_oz;  // Origins
    int m_size = 0;
    std::vector<std::future<std::unique_ptr<Chunk>>> m_chunks_to_build;
    opticrafter::RingBuffer2D<std::unique_ptr<Chunk>> m_chunks;

    std::queue<Chunk*> m_dirty;
    std::queue<glm::ivec2> m_invalidated;
    mutable std::shared_mutex m_mutex;

    // Created to avoid dead lock when using getChunk inside a locked method (ex. setOrigin())
    Chunk* getChunkNoLock(int cx, int cz) const;
};