// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Mathf.h"

#include <cmath>
#include <limits>

namespace Theodore {
  const float Mathf::pi = 3.1415926f;
  const int Mathf::degrees = 360;
  const float Mathf::degrees_to_radians = Mathf::pi / 180.f;
  const float Mathf::radians_to_degrees = 180.f / Mathf::pi;
  const float Mathf::flt_epsilon = std::numeric_limits<float>::epsilon();
  const float Mathf::infinity = std::numeric_limits<float>::infinity();

  bool Mathf::IsEqual(const float a, const float b) { return std::fabs(a - b) < Mathf::flt_epsilon; }

  bool Mathf::IsZero(const float a) { return std::fabsf(a) <= 0.000001f; }

  float Mathf::Clamp(float x, float min, float max) { return x < min ? min : (x > max ? max : x); }

  int Mathf::Clamp(int x, int min, int max) { return x < min ? min : (x > max ? max : x); }

  float Mathf::Clamp01(float value) { return value < 0.f ? 0.f : (value > 1.f ? 1.f : value); }

  float Mathf::Lerp(float a, float b, float t) { return a + (b - a) * Mathf::Clamp01(t); }

  float Mathf::LerpUnclamped(float a, float b, float t) { return a + (b - a) * t; }

  float Mathf::LerpAngle(float a, float b, float t) {
    float num = Mathf::DeltaAngle(b - a, 360.f);
    return a + num * Mathf::Clamp01(t);
  }

  float Mathf::InverseLerp(float a, float b, float value) {
    if (Mathf::IsEqual(a, b)) {
      return 0.f;
    }
    return Mathf::Clamp01((value - a) / (b - a));
  }

  float Mathf::Radians(float degrees) { return degrees * degrees_to_radians; }

  float Mathf::Degrees(float radians) { return radians * radians_to_degrees; }

  float Mathf::Sign(float f) { return f >= 0.f ? 1.f : -1.f; }

  float Mathf::Repeat(float t, float length) {
    /*
      it is never larger than length and never smaller than 0.
      Math::Reapeat(-100, 360) -> 260
      Math::Reapeat(370, 360) -> 10
      Math::Reapeat(250, 360) -> 250
    */
    return t - std::floorf(t / length) * length;
  }

  float Mathf::DeltaAngle(float current, float target) {
    float num = Mathf::Repeat(target - current, 360.f);
    if (num > 180.f) {
      num -= 360.f;
    }
    return num;
  }

  float Mathf::PingPong(float t, float len) {
    float L = 2 * len;
    float T = fmodf(t, L);

    if (0 <= T && T < len) {
      return T;
    } else {
      return (L - T);
    }
  }

  float Mathf::ConvertKilloToMega(int kb) { return kb / 1024.f; }

  float Mathf::PerlinNoise(float x) {
    // TODO
    return 0.f;
  }

  float Mathf::PerlinNoise(float x, float y) {
    // TODO
    return 0.f;
  }

  float Mathf::PerlinNoise(float x, float y, float z) {
    // TODO
    return 0.f;
  }

  float Mathf::SmoothStep(float min, float max, float value) {
    float x = std::fmaxf(0.f, std::fminf(1.f, (value - min) / (max - min)));
    return x * x * (3 - 2 * x);
  }

  float Mathf::MoveTowards(float current, float target, float maxDelta) {
    if (std::fabsf(target - current) <= maxDelta) {
      return target;
    }
    return current + Mathf::Sign(target - current) * maxDelta;
  }

  float Mathf::MoveTowardsAngle(float current, float target, float maxDelta) {
    target = current + Mathf::DeltaAngle(current, target);
    return Mathf::MoveTowards(current, target, maxDelta);
  }

  float Mathf::EaseIn() {
    // TODO
    return 0.f;
  }

  float Mathf::Sin(float angle) { return std::sin(angle); }

  float Mathf::Cos(float angle) { return std::cos(angle); }

  float Mathf::Tan(float angle) { return std::tan(angle); }

  float Mathf::Asin(float angle) { return std::asin(angle); }

  float Mathf::Acos(float angle) { return std::acos(angle); }

  float Mathf::Atan(float angle) { return std::atan(angle); }

  float Mathf::Ceil(float a) { return std::ceilf(a); }

  float Mathf::Floor(float a) { return std::floorf(a); }

  float Mathf::Round(float a) { return a < 0.f ? std::ceilf(a - 0.5f) : std::floorf(a + 0.5f); }

  float Mathf::Abs(float a) { return std::fabsf(a); }

  float Mathf::Log(float f) { return std::log(f); }

  float Mathf::Log10(float f) { return std::log10(f); }

  float Mathf::Exp(float f) { return std::exp(f); }

  float Mathf::Sqrt(float f) { return std::sqrtf(f); }

  float Mathf::Max(float a, float b) { return std::fmaxf(a, b); }

  float Mathf::Min(float a, float b) { return std::fminf(a, b); }

  float Mathf::Dot(const Vector3d& v1, const Vector3d& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

  float Mathf::AbsDot(const Vector3d& v1, const Vector3d& v2) { return std::fabsf(v1.x * v2.x) + std::fabsf(v1.y * v2.y) + std::fabsf(v1.z * v2.z); }

  Vector3d Mathf::Pow(const Vector3d& a, float exp) { return Vector3d(std::powf(a.x, exp), std::powf(a.x, exp), std::powf(a.x, exp)); }

  float Mathf::Dot(const Vector2d& v1, const Vector2d& v2) { return v1.x * v2.x + v1.y * v2.y; }

  float Mathf::AbsDot(const Vector2d& v1, const Vector2d& v2) { return std::fabsf(v1.x * v2.x) + std::fabsf(v1.y * v2.y); }

  Vector2d Mathf::Pow(const Vector2d& a, float exp) { return Vector2d(std::powf(a.x, exp), std::powf(a.x, exp)); }
}  // namespace Theodore
