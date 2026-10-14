#pragma once
#include <SDL3/SDL.h>
#include <cstdint>

class RandomGenerator {
    public:
        Uint64 rngSeed;
        void seed(uint64_t seedInt);
        int32_t rand(int32_t max, int32_t min = 0);
        float randf(float max = 1, float min = 0);
};
