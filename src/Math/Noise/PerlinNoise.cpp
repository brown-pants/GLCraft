#include "PerlinNoise.h"

FastNoiseLite PerlinNoise::fastNoise;

void PerlinNoise::Init(int seed)
{
    fastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    fastNoise.SetSeed(seed);
}

float PerlinNoise::Noise_2d(float freq, float amplitude, float x, float y)
{
    return fastNoise.GetNoise(freq * x, freq * y) * amplitude;
}

float PerlinNoise::Noise_3d(float freq, float amplitude, float x, float y, float z)
{
    return fastNoise.GetNoise(freq * x, freq * y, freq * z) * amplitude;
}
