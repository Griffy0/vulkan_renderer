#include "RandomGenerator.hpp"
#include <SDL3/SDL_stdinc.h>
#include <cmath>

void RandomGenerator::seed(uint64_t seedInt) {
    SDL_srand(seedInt); // This will cause issues with multithreading, store the seed and use sdl_rand_r with state references instead then
};

int32_t RandomGenerator::rand(int32_t max, int32_t min) {
    // Exclusive max, inclusive min
    return SDL_rand(max-min) + min;
};

float RandomGenerator::randf(float max, float min) {
    // Exclusive max, inclusive min
    return SDL_randf() * (max - min) - min;
};
