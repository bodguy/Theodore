#include "Random.h"
#include <cstdlib>

namespace Theodore {
  void Random::InitState(int seed) { std::srand(seed); }

  float Random::Range(float min, float max) { return min + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (max - min)))); }

  int Random::Range(int min, int max) { return min + std::rand() / (RAND_MAX / (max - min)); }
} // namespace Theodore
