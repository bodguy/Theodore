/**
  @brief common math utility
  @author bodguy
  @date 17.07.17
  @todo EaseIn(), PerlinNoise() function implementation
  @bug
*/

#ifndef Math_h
#define Math_h

#include "Vector2d.h"
#include "Vector3d.h"

namespace Theodore {
  class Math {
  public:
    static bool IsEqual(const float a, const float b);
    static bool IsZero(const float a);
    static float Clamp(float x, float min, float max);
    static int Clamp(int x, int min, int max);
    static float Clamp01(float value);
    static float Lerp(float a, float b, float t);
    static float LerpUnclamped(float a, float b, float t);
    static float LerpAngle(float a, float b, float t);
    static float InverseLerp(float a, float b, float value);
    static float Radians(float degrees);
    static float Degrees(float radians);
    static float Sign(float f);
    static float Repeat(float t, float length);
    static float DeltaAngle(float current, float target);
    static float PingPong(float t, float len);
    static float KbToMb(int kb);
    static float PerlinNoise(float x);
    static float PerlinNoise(float x, float y);
    static float PerlinNoise(float x, float y, float z);
    static float SmoothStep(float min, float max, float value);
    static float MoveTowards(float current, float target, float maxDelta);
    static float MoveTowardsAngle(float current, float target, float maxDelta);
    static float EaseIn();

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
    static float Log(float f);
    static float Log10(float f);
    static float Exp(float f);
    static float Sqrt(float f);

    static float Max(float a, float b);
    static float Min(float a, float b);

    static float Dot(const Vector3d& v1, const Vector3d& v2);
    static float AbsDot(const Vector3d& v1, const Vector3d& v2);
    static Vector3d Pow(const Vector3d& a, float exp);

    static float Dot(const Vector2d& v1, const Vector2d& v2);
    static float AbsDot(const Vector2d& v1, const Vector2d& v2);
    static Vector2d Pow(const Vector2d& a, float exp);

    static const float pi;
    static const int degrees;
    static const float degrees_to_radians;
    static const float radians_to_degrees;
    static const float flt_epsilon;
    static const float infinity;
  };
}

#endif /* Math_h */
