#ifndef Random_h
#define Random_h

namespace Theodore {
    class Random {
    public:
        static void InitState(int seed);
        static float Range(float min, float max); // (min, max]
        static int Range(int min, int max); // (min, max]
    };
}

#endif /* Random_h */
