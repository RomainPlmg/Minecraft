#pragma once

#include <FastNoiseLite.h>

class TerrainGenerator {
   public:
    TerrainGenerator(int seed = 0xdeadbeef);

    int getHeight(int wx, int wz) const;

   private:
    FastNoiseLite m_noise;
};
