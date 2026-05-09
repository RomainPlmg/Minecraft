#include "TerrainGenerator.h"

#include "Chunk.h"

TerrainGenerator::TerrainGenerator(int seed) {
    m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_noise.SetFrequency(0.005f);
    m_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noise.SetFractalOctaves(4);
    m_noise.SetSeed(seed);
}

int TerrainGenerator::getHeight(int wx, int wz) const {
    float n = m_noise.GetNoise((float)wx, (float)wz);  // [-1, 1]
    return (int)((n + 1.f) / 2.f * Chunk::CHUNK_HEIGHT);
}