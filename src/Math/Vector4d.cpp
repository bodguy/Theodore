#include "Vector4d.h"
#include "Math.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include <algorithm> // until c++11 for std::swap
#include <cmath>
#include <utility> // since c++11 for std::swap

namespace Theodore {
  Vector4d::Vector4d() : x(0.f), y(0.f), z(0.f), w(1.f) {}

  Vector4d::Vector4d(float tx, float ty, float tz, float tw) : x(tx), y(ty), z(tz), w(tw) {}

  Vector4d::Vector4d(const Vector2d& other) {
    x = other.x;
    y = other.y;
    z = 0.0f;
    w = 0.0f;
  }

  Vector4d::Vector4d(const Vector3d& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 0.0f;
  }

  Vector4d::Vector4d(const Vector2d& other, float tz, float tw) {
    x = other.x;
    y = other.y;
    z = tz;
    w = tw;
  }

  Vector4d::Vector4d(const Vector3d& other, float tw) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = tw;
  }

  Vector4d::Vector4d(const Vector4d& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  Vector4d& Vector4d::operator=(Vector4d other) {
    Swap(*this, other);
    return *this;
  }

  float Vector4d::operator[](unsigned int i) const {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      return x;
    }
  }

  Vector4d Vector4d::operator+(const Vector4d& other) const {
    // using op= (more effective c++ section 22)
    return Vector4d(*this) += other;
  }

  Vector4d& Vector4d::operator+=(const Vector4d& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
  }

  Vector4d Vector4d::operator-(const Vector4d& other) const {
    // using op= (more effective c++ section 22)
    return Vector4d(*this) -= other;
  }

  Vector4d& Vector4d::operator-=(const Vector4d& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
  }

  Vector4d Vector4d::operator*(const Vector4d& other) const {
    // using op= (more effective c++ section 22)
    return Vector4d(*this) *= other;
  }

  Vector4d& Vector4d::operator*=(const Vector4d& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;

    return *this;
  }

  Vector4d Vector4d::operator/(const Vector4d& other) const {
    // using op= (more effective c++ section 22)
    return Vector4d(*this) /= other;
  }

  Vector4d& Vector4d::operator/=(const Vector4d& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;

    return *this;
  }

  float Vector4d::DotProduct(const Vector4d& a, const Vector4d& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

  Vector4d Vector4d::operator+(const float scalar) const { return Vector4d(*this) += scalar; }

  Vector4d& Vector4d::operator+=(const float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;

    return *this;
  }

  Vector4d Vector4d::operator-(const float scalar) const { return Vector4d(*this) -= scalar; }

  Vector4d& Vector4d::operator-=(const float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;

    return *this;
  }

  Vector4d Vector4d::operator*(const float scalar) const { return Vector4d(*this) *= scalar; }

  Vector4d& Vector4d::operator*=(const float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
  }

  Vector4d Vector4d::operator/(const float scalar) const { return Vector4d(*this) /= scalar; }

  Vector4d& Vector4d::operator/=(const float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;
  }

  bool Vector4d::operator<(const Vector4d& other) const { return x < other.x && y < other.y && z < other.z && w < other.w; }

  bool Vector4d::operator>(const Vector4d& other) const { return x > other.x && y > other.y && z > other.z && w > other.w; }

  bool Vector4d::operator<=(const Vector4d& other) const { return x <= other.x && y <= other.y && z <= other.z && w <= other.w; }

  bool Vector4d::operator>=(const Vector4d& other) const { return x >= other.x && y >= other.y && z >= other.z && w >= other.w; }

  bool Vector4d::operator==(const Vector4d& other) const { return (Math::IsEqual(x, other.x) && Math::IsEqual(y, other.y) && Math::IsEqual(z, other.z) && Math::IsEqual(w, other.w)); }

  bool Vector4d::operator!=(const Vector4d& other) const { return !(*this == other); }

  bool Vector4d::operator<(const float scalar) const { return x < scalar && y < scalar && z < scalar; }

  bool Vector4d::operator>(const float scalar) const { return x > scalar && y > scalar && z > scalar; }

  bool Vector4d::operator<=(const float scalar) const { return x <= scalar && y <= scalar && z <= scalar; }

  bool Vector4d::operator>=(const float scalar) const { return x >= scalar && y >= scalar && z >= scalar; }

  bool Vector4d::operator==(const float scalar) const {
    // scalar only equal with vector components.
    return (Math::IsEqual(x, scalar) && Math::IsEqual(y, scalar) && Math::IsEqual(z, scalar));
  }

  bool Vector4d::operator!=(const float scalar) const { return !(*this == scalar); }

  Vector4d& Vector4d::Normalize() {
    float len = std::sqrt(x * x + y * y + z * z + w * w);
    if (Math::IsZero(len) || Math::IsEqual(len, 1.f))
      return *this;

    float inv = 1 / len;

    x = x * inv;
    y = y * inv;
    z = z * inv;

    return *this;
  }

  float Vector4d::Length() { return std::sqrt(x * x + y * y + z * z + w * w); }

  Vector3d Vector4d::ToVector3d(const Vector4d& other) { return Vector3d(other.x, other.y, other.z); }

  Vector4d Vector4d::Absolute(const Vector4d& other) { return Vector4d(std::fabsf(other.x), std::fabsf(other.y), std::fabsf(other.z), std::fabsf(other.w)); }

  void Vector4d::Swap(Vector4d& first, Vector4d& second) {
    using std::swap;
    swap(first.x, second.x);
    swap(first.y, second.y);
    swap(first.z, second.z);
    swap(first.w, second.w);
  }

  const Vector4d Vector4d::up = Vector4d(0.f, 1.f, 0.f, 1.f);
  const Vector4d Vector4d::down = Vector4d(0.f, -1.f, 0.f, 1.f);
  const Vector4d Vector4d::left = Vector4d(-1.f, 0.f, 0.f, 1.f);
  const Vector4d Vector4d::right = Vector4d(1.f, 0.f, 0.f, 1.f);
  const Vector4d Vector4d::forward = Vector4d(0.f, 0.f, -1.f, 1.f);
  const Vector4d Vector4d::backward = Vector4d(0.f, 0.f, 1.f, 1.f);
  const Vector4d Vector4d::one = Vector4d(1.f, 1.f, 1.f, 1.f);
  const Vector4d Vector4d::zero = Vector4d(0.f, 0.f, 0.f, 0.f);
} // namespace Theodore
