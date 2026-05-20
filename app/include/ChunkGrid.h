#pragma once

#include "Chunk.h"

class ChunkGrid {
   public:
    ChunkGrid(opticrafter::ThreadPool& thread_pool, uint8_t build_distance, int ox = 0, int oz = 0);

    [[nodiscard]] std::shared_ptr<Chunk> getChunk(int cx, int cz) const;
    [[nodiscard]] std::optional<BlockType> getBlock(int x, int y, int z) const;
    bool isInBounds(int cx, int cz) const;
    bool isBoundary(int cx, int cz) const;

    void update(opticrafter::ThreadPool& thread_pool, int ox, int oz);

    [[nodiscard]] std::vector<std::shared_ptr<Chunk>> pollChunksToMesh(size_t max);
    [[nodiscard]] std::vector<glm::ivec2> pollInvalidatedChunks();
    void pollPendingChunks();

   private:
    int m_ox, m_oz;  // Origins
    int m_size = 0;
    opticrafter::TSQueue<std::shared_ptr<Chunk>> m_chunks_built;
    opticrafter::RingBuffer2D<std::shared_ptr<Chunk>> m_chunks;

    std::queue<std::shared_ptr<Chunk>> m_dirty;
    std::queue<glm::ivec2> m_invalidated;
    mutable std::shared_mutex m_mutex;

    // Created to avoid dead lock when using getChunk inside a locked method (ex. setOrigin())
    std::shared_ptr<Chunk> getChunkNoLock(int cx, int cz) const;
};