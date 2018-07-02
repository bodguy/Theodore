#ifndef Math_h
#define Math_h

namespace Quark {
	template <int N>
	struct Factorial
	{
		enum { value = N * Factorial<N - 1>::value };
	};

	template <>
	struct Factorial<0>
	{
		enum { value = 1 };
	};

    class Math {
    public:
        static bool IsEqual(const float a, const float b);
        static bool IsZero(const float a);
        static float Clamp(float x, float a, float b);
        static float Lerp(float a, float b, float t);
        static float Radians(float degrees);
        static float Degrees(float radians);
		static float PingPong(float t, float len);
		static float KbToMb(int kb);
		static float PerlinNoise(float x);
		static float PerlinNoise(float x, float y);
        static float PerlinNoise(float x, float y, float z);
		static float SmoothStep(float min, float max, float value);
		//static float EaseIn();

        static float Sin(float angle);
        static float Cos(float angle);
        static float Tan(float angle);
        static float Asin(float angle);
        static float Acos(float angle);
        static float Atan(float angle);

		static float Ceil(float a);
		static float Floor(float a);
		static float Round(float a);
		static float Abs(float a);
        
        static const float pi;
        static const int degrees;
        static const float degrees_to_radians;
		static const float flt_epsilon;
		static const float infinity;
    };
}

#endif /* Math_h */
