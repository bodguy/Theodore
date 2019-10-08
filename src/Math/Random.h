// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Random_h
#define Random_h

namespace Theodore {
  class Random {
  public:
    static void InitState(int seed);
    static float Range(float min, float max);  // (min, max]
    static int Range(int min, int max);        // (min, max]
  };
}  // namespace Theodore

#endif /* Random_h */
