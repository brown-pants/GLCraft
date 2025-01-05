#ifndef __PERLINNOISE_H__
#define __PERLINNOISE_H__
#include <FastNoiseLite.h>

class PerlinNoise
{
public:
    static void Init(int seed);
    static float Noise_2d(float freq, float amplitude, float x, float y);
    static float Noise_3d(float freq, float amplitude, float x, float y, float z);

private:
    static FastNoiseLite fastNoise;
};

#endif