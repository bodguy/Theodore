#include "Math.h"
#include <cmath>
#include <float.h>

namespace Quark {
    const float Math::pi = 3.1415926f;
    const int Math::degrees = 360;
    const float Math::degrees_to_radians = Math::pi / 180.f;
    
    bool Math::IsEqual(const float a, const float b) {
        return std::abs(a - b) < FLT_EPSILON;
    }
    
    bool Math::IsZero(const float a) {
        return std::abs(a) <= 0.000001f;
    }
    
    float Math::Clamp(float x, float a, float b) {
        return x < a ? a : (x > b ? b : x);
    }
    
    float Math::Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }
    
    float Math::Radians(float degrees) {
        return degrees * degrees_to_radians;
    }
    
    float Math::Degrees(float radians) {
        return radians * (1 / degrees_to_radians);
    }
    
    float Math::Sin(float angle) {
        return std::sin(angle);
    }
    
    float Math::Cos(float angle) {
        return std::cos(angle);
    }
    
    float Math::Tan(float angle) {
        return std::tan(angle);
    }
    
    float Math::Asin(float angle) {
        return std::asin(angle);
    }
    
    float Math::Acos(float angle) {
        return std::acos(angle);
    }
    
    float Math::Atan(float angle) {
        return std::atan(angle);
    }
}
