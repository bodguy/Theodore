#include "Math.h"
#include <cmath>
#include <float.h>
#include <limits>

namespace Theodore {
    const float Math::pi = 3.1415926f;
    const int Math::degrees = 360;
    const float Math::degrees_to_radians = Math::pi / 180.f;
    const float Math::flt_epsilon = std::numeric_limits<float>::epsilon();
	const float Math::infinity = std::numeric_limits<float>::infinity();
    
    bool Math::IsEqual(const float a, const float b) {
       return std::fabs(a - b) < Math::flt_epsilon;
    }
    
    bool Math::IsZero(const float a) {
        return std::fabsf(a) <= 0.000001f;
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

	float Math::PingPong(float t, float len) {
		float L = 2 * len;
		float T = fmodf(t, L);

		if(0 <= T && T < len) {
			return T;
		} else {
			return (L - T);
		}
	}

	float Math::KbToMb(int kb) {
		return kb / 1024.f;
	}

	float Math::PerlinNoise(float x) {
		// TODO
		return 0.f;
	}

	float Math::PerlinNoise(float x, float y) {
		// TODO
		return 0.f;
	}

    float Math::PerlinNoise(float x, float y, float z) {
		// TODO
		return 0.f;
	}

	float Math::SmoothStep(float min, float max, float value) {
		float x = std::fmaxf(0.f, std::fminf(1.f, (value - min) / (max - min)));
		return x * x * (3 - 2 * x);
	}

	float Math::EaseIn() {
		// TODO
		return 0.f;
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

	float Math::Ceil(float a) {
		return std::ceilf(a);
	}

	float Math::Floor(float a) {
		return std::floorf(a);
	}

	float Math::Round(float a) {
		return a < 0.f ? std::ceilf(a - 0.5f) : std::floorf(a + 0.5f);
	}

	float Math::Abs(float a) {
		return std::fabsf(a);
	}

	float Math::Max(float a, float b) {
		return std::fmaxf(a, b);
	}

	float Math::Min(float a, float b) {
		return std::fminf(a, b);
	}

	float Math::Dot(const Vector3d& v1, const Vector3d& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Math::AbsDot(const Vector3d& v1, const Vector3d& v2) {
		return std::fabsf(v1.x * v2.x) + std::fabsf(v1.y * v2.y) + std::fabsf(v1.z * v2.z);
	}

	Vector3d Math::Power(const Vector3d& a, float exp) {
		return Vector3d(std::powf(a.x, exp), std::powf(a.x, exp), std::powf(a.x, exp));
	}
}
