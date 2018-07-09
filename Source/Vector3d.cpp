#include "Vector3d.h"
#include "Vector2d.h"
#include "Math.h"
#include <cmath>
#include <algorithm> // until c++11 for std::swap
#include <utility> // since c++11 for std::swap

namespace Quark {
    Vector3d::Vector3d() :x(0.f), y(0.f), z(0.f) {
        
    }
    
    Vector3d::Vector3d(const float tx, const float ty, const float tz) {
        x = tx; y = ty; z = tz;
    }
    
    Vector3d::Vector3d(const Vector2d& other) {
        x = other.x;
        y = other.y;
        z = 0.f;
    }
    
    Vector3d::Vector3d(const Vector2d& other, float z) {
        x = other.x;
        y = other.y;
        z = z;
    }
    
    Vector3d::Vector3d(const float tx, const float ty) {
        x = tx; y = ty; z = 0.f;
    }
    
    Vector3d::Vector3d(const Vector3d& other) {
        x = other.x; y = other.y; z = other.z;
    }
    
    Vector3d& Vector3d::operator = (Vector3d other) {
        // copy and swap idiom (effective c++ section 11)
        Swap(*this, other);
        return *this;
    }

	float Vector3d::operator[] (unsigned int i) const {
		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return x;
		}
	}
    
    Vector3d Vector3d::operator +(const Vector3d& other) const {
        // using op= (more effective c++ section 22)
        return Vector3d(*this) += other;
    }
    
    Vector3d& Vector3d::operator +=(const Vector3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    
    Vector3d Vector3d::operator -(const Vector3d& other) const {
        return Vector3d(*this) -= other;
    }
    
    Vector3d& Vector3d::operator -=(const Vector3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    
    Vector3d Vector3d::operator *(const Vector3d& other) const {
        return Vector3d(*this) *= other;
    }
    
    Vector3d& Vector3d::operator *=(const Vector3d& other) {
        x *= other.x; y *= other.y; z *= other.z;
        return *this;
    }
    
    Vector3d Vector3d::operator /(const Vector3d& other) const {
        return Vector3d(*this) /= other;
    }
    
    Vector3d& Vector3d::operator /=(const Vector3d& other) {
        x /= other.x; y /= other.y; z /= other.z;
        return *this;
    }
    
    Vector3d Vector3d::CrossProduct(const Vector3d& a, const Vector3d& b) {
        return Vector3d(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }
    
    float Vector3d::DotProduct(const Vector3d& a, const Vector3d& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    Vector3d Vector3d::operator +(const float scalar) const {
        return Vector3d(*this) += scalar;
    }
    
    Vector3d& Vector3d::operator +=(const float scalar) {
        x += scalar; y += scalar; z += scalar;
        return *this;
    }
    
    Vector3d Vector3d::operator -(const float scalar) const {
        return Vector3d(*this) -= scalar;
    }
    
    Vector3d& Vector3d::operator -=(const float scalar) {
        x -= scalar; y -= scalar; z -= scalar;
        return *this;
    }
    
    Vector3d Vector3d::operator *(const float scalar) const {
        return Vector3d(*this) *= scalar;
    }
    
    Vector3d& Vector3d::operator *=(const float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
    
    Vector3d Vector3d::operator /(const float scalar) const {
        return Vector3d(*this) /= scalar;
    }
    
    Vector3d& Vector3d::operator /=(const float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }
    
    bool Vector3d::operator <(const Vector3d& other) const {
        return x < other.x && y < other.y && z < other.z;
    }
    
    bool Vector3d::operator >(const Vector3d& other) const {
        return x > other.x && y > other.y && z > other.z;
    }
    
    bool Vector3d::operator <=(const Vector3d& other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }
    
    bool Vector3d::operator >=(const Vector3d& other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }
    
    bool Vector3d::operator ==(const Vector3d& other) const {
        return (Math::IsEqual(x, other.x) && Math::IsEqual(y, other.y) && Math::IsEqual(z, other.z));
    }
    
    bool Vector3d::operator !=(const Vector3d& other) const {
        return !(*this == other);
    }
    
    bool Vector3d::operator <(const float scalar) const {
        return x < scalar && y < scalar && z < scalar;
    }
    
    bool Vector3d::operator >(const float scalar) const {
        return x > scalar && y > scalar && z > scalar;
    }
    
    bool Vector3d::operator <=(const float scalar) const {
        return x <= scalar && y <= scalar && z <= scalar;
    }
    
    bool Vector3d::operator >=(const float scalar) const {
        return x >= scalar && y >= scalar && z >= scalar;
    }
    
    bool Vector3d::operator ==(const float scalar) const {
        return (Math::IsEqual(x, scalar) && Math::IsEqual(y, scalar) && Math::IsEqual(z, scalar));
    }
    
    bool Vector3d::operator !=(const float scalar) const {
        return !(*this == scalar);
    }
    
    Vector3d Vector3d::operator -() const {
        return Vector3d(-x, -y, -z);
    }
    
    Vector3d Vector3d::operator +() const {
        return Vector3d(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z);
    }
    
    Vector3d& Vector3d::Negate() {
        x = -x; y = -y; z = -z;
        return *this;
    }
    
    Vector3d Vector3d::Inverse(const Vector3d& other) {
        return Vector3d(1.f / other.x, 1.f / other.y, 1.f / other.z);
    }
    
    Vector3d Vector3d::Perpendicular() {
        return Vector3d(-y, x, z);
    }
    
    Vector3d& Vector3d::Normalize() {
        float len = std::sqrt(x * x + y * y + z * z);
        if (Math::IsZero(len) || Math::IsEqual(len, 1.f))
            return *this;
        
        float inv = 1 / len;
        
        x = x * inv; y = y * inv; z = z * inv;
        
        return *this;
    }
    
    float Vector3d::Magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float Vector3d::Distance(const Vector3d& other) const {
        return std::sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z));
    }
    
    float Vector3d::DistanceSqrare(const Vector3d& other) const {
        Vector3d c = *this - other;
        return Vector3d::DotProduct(c, c);
    }
    
    Vector3d Vector3d::Lerp(const Vector3d& a, const Vector3d& b, float t) {
        return (a + (b - a) * t);
    }
    
    Vector3d Vector3d::Slerp(const Vector3d& a, const Vector3d& b, float t) {
        // Dot product - the cosine of the angle between 2 vectors.
        float dot = Vector3d::DotProduct(a, b);
        Math::Clamp(dot, -1.0f, 1.0f);
        
        // Acos(dot) returns the angle between start and end,
        // And multiplying that by t returns the angle between
        // start and the final result.
        float theta = std::acos(dot) * t;
        Vector3d rv = b - a * dot;
        rv.Normalize();
        
        return ((a*std::cos(theta)) + (rv*std::sin(theta)));
    }
    
    Vector3d Vector3d::Nlerp(const Vector3d& a, const Vector3d& b, float t) {
        Vector3d lerp = Lerp(a, b, t);
        lerp.Normalize();
        return lerp;
    }

	Vector3d Vector3d::Project(const Vector3d& vector, const Vector3d& onNormal) {
		float num = Vector3d::DotProduct(onNormal, onNormal);
		if(num < Math::flt_epsilon) {
			return Vector3d::zero;
		}
		return onNormal * Vector3d::DotProduct(vector, onNormal) / num;
	}

	Vector3d Vector3d::ProjectOnPlane(const Vector3d& vector, const Vector3d& planeNormal) {
		return vector - Vector3d::Project(vector, planeNormal);
	}
    
    void Vector3d::Swap(Vector3d& first, Vector3d& second) {
        using std::swap;
        
        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.z, second.z);
    }
    
    const Vector3d Vector3d::up = Vector3d(0.f, 1.f, 0.f);
    const Vector3d Vector3d::down = Vector3d(0.f, -1.f, 0.f);
    const Vector3d Vector3d::left = Vector3d(-1.f, 0.f, 0.f);
    const Vector3d Vector3d::right = Vector3d(1.f, 0.f, 0.f);
    const Vector3d Vector3d::forward = Vector3d(0.f, 0.f, -1.f);
    const Vector3d Vector3d::backward = Vector3d(0.f, 0.f, 1.f);
    const Vector3d Vector3d::one = Vector3d(1.f, 1.f, 1.f);
    const Vector3d Vector3d::zero = Vector3d(0.f, 0.f, 0.f);
}
