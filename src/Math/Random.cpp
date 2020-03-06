// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Random.h"
#include <cstdlib>

namespace Theodore {
  void Random::InitState(int seed) { std::srand(seed); }

  float Random::Range(float min, float max) { return min + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (max - min)))); }

  int Random::Range(int min, int max) { return min + std::rand() / (RAND_MAX / (max - min)); }
}  // namespace Theodore
