#include "Vector2d.h"
#include "Math.h"
#include <algorithm> // until c++11 for std::swap
#include <cmath>
#include <utility> // since c++11 for std::swap

namespace Theodore {
  Vector2d::Vector2d() : x(0.f), y(0.f) {}

  Vector2d::Vector2d(const float fx, const float fy) : x(fx), y(fy) {}

  Vector2d::Vector2d(const Vector2d& other) {
    x = other.x;
    y = other.y;
  }

  Vector2d::Vector2d(const Vector3d& other) {
    x = other.x;
    y = other.y;
  }

  Vector2d& Vector2d::operator=(Vector2d other) {
    // copy and swap idiom (effective c++ section 11)
    Swap(*this, other);
    return *this;
  }

  float Vector2d::operator[](unsigned int i) const {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return x;
    }
  }

  Vector2d Vector2d::operator+(const Vector2d& other) const {
    // using op= (more effective c++ section 22)
    return Vector2d(*this) += other;
  }

  Vector2d& Vector2d::operator+=(const Vector2d& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector2d Vector2d::operator-(const Vector2d& other) const { return Vector2d(*this) -= other; }

  Vector2d& Vector2d::operator-=(const Vector2d& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2d Vector2d::operator*(const Vector2d& other) const { return Vector2d(*this) *= other; }

  Vector2d& Vector2d::operator*=(const Vector2d& other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  Vector2d Vector2d::operator/(const Vector2d& other) const { return Vector2d(*this) /= other; }

  Vector2d& Vector2d::operator/=(const Vector2d& other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  float Vector2d::CrossProduct(const Vector2d& a, const Vector2d& b) { return a.x * b.y - a.y * b.x; }

  float Vector2d::DotProduct(const Vector2d& a, const Vector2d& b) { return a.x * b.x + a.y * b.y; }

  Vector2d Vector2d::operator+(const float scalar) const { return Vector2d(*this) += scalar; }

  Vector2d& Vector2d::operator+=(const float scalar) {
    x += scalar;
    y += scalar;
    return *this;
  }

  Vector2d Vector2d::operator-(const float scalar) const { return Vector2d(*this) -= scalar; }

  Vector2d& Vector2d::operator-=(const float scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
  }

  Vector2d Vector2d::operator*(const float scalar) const { return Vector2d(*this) *= scalar; }

  Vector2d& Vector2d::operator*=(const float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vector2d Vector2d::operator/(const float scalar) const { return Vector2d(*this) /= scalar; }

  Vector2d& Vector2d::operator/=(const float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  bool Vector2d::operator<(const Vector2d& other) const { return x < other.x && y < other.y; }

  bool Vector2d::operator>(const Vector2d& other) const { return x > other.x && y > other.y; }

  bool Vector2d::operator<=(const Vector2d& other) const { return x <= other.x && y <= other.y; }

  bool Vector2d::operator>=(const Vector2d& other) const { return x >= other.x && y >= other.y; }

  bool Vector2d::operator==(const Vector2d& other) const { return (Math::IsEqual(x, other.x) && Math::IsEqual(y, other.y)); }

  bool Vector2d::operator!=(const Vector2d& other) const { return !(*this == other); }

  bool Vector2d::operator<(const float scalar) const { return x < scalar && y < scalar; }

  bool Vector2d::operator>(const float scalar) const { return x > scalar && y > scalar; }

  bool Vector2d::operator<=(const float scalar) const { return x <= scalar && y <= scalar; }

  bool Vector2d::operator>=(const float scalar) const { return x >= scalar && y >= scalar; }

  bool Vector2d::operator==(const float scalar) const { return (Math::IsEqual(x, scalar) && Math::IsEqual(y, scalar)); }

  bool Vector2d::operator!=(const float scalar) const { return !(*this == scalar); }

  Vector2d Vector2d::operator-() const { return Vector2d(-x, -y); }

  Vector2d Vector2d::operator+() const { return Vector2d(x < 0 ? -x : x, y < 0 ? -y : y); }

  Vector2d Vector2d::Perpendicular() { return Vector2d(-y, x); }

  Vector2d& Vector2d::Normalize() {
    float len = std::sqrt(x * x + y * y);
    if (Math::IsZero(len) || Math::IsEqual(len, 1.f))
      return *this;

    float inv = 1 / len;

    x = x * inv;
    y = y * inv;

    return *this;
  }

  float Vector2d::Length() { return std::sqrt(x * x + y * y); }

  float Vector2d::Distance(const Vector2d& other) { return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)); }

  float Vector2d::DistanceSqrare(const Vector2d& other) {
    Vector2d c = *this - other;
    return Vector2d::DotProduct(c, c);
  }

  Vector2d Vector2d::Absolute(const Vector2d& other) { return Vector2d(std::fabsf(other.x), std::fabsf(other.y)); }

  Vector2d Vector2d::Lerp(const Vector2d& a, const Vector2d& b, float t) { return (a + (b - a) * t); }

  Vector2d Vector2d::Slerp(const Vector2d& a, const Vector2d& b, float t) {
    // Dot product - the cosine of the angle between 2 vectors.
    float dot = Vector2d::DotProduct(a, b);
    // Math::Clamp(dot, -1.0f, 1.0f);

    // Acos(dot) returns the angle between start and end,
    // And multiplying that by t returns the angle between
    // start and the final result.
    float theta = std::acos(dot) * t;
    Vector2d rv = b - a * dot;
    rv.Normalize();

    return ((a * std::cos(theta)) + (rv * std::sin(theta)));
  }

  Vector2d Vector2d::Nlerp(const Vector2d& a, const Vector2d& b, float t) {
    Vector2d lerp = Vector2d::Lerp(a, b, t);
    lerp.Normalize();
    return lerp;
  }

  Vector2d Vector2d::Normalize(const Vector2d& vector) {
    float len = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (Math::IsZero(len) || Math::IsEqual(len, 1.f))
      return vector;

    float inv = 1 / len;
    return Vector2d(vector.x * inv, vector.y * inv);
  }

  void Vector2d::Swap(Vector2d& fisrt, Vector2d& second) {
    using std::swap;
    swap(fisrt.x, second.x);
    swap(fisrt.y, second.y);
  }

  const Vector2d Vector2d::up = Vector2d(0.f, 1.f);
  const Vector2d Vector2d::down = Vector2d(0.f, -1.f);
  const Vector2d Vector2d::left = Vector2d(-1.f, 0.f);
  const Vector2d Vector2d::right = Vector2d(1.f, 0.f);
  const Vector2d Vector2d::one = Vector2d(1.f, 1.f);
  const Vector2d Vector2d::zero = Vector2d(0.f, 0.f);
} // namespace Theodore
