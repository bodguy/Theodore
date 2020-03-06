// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Vector2d_h
#define Vector2d_h

namespace Theodore {
  class Vector3d;
  class Vector2d {
  public:
    Vector2d();
    Vector2d(const float tx, const float ty);
    Vector2d(const Vector2d& other);
    Vector2d(const Vector3d& other);
    Vector2d& operator=(Vector2d other);
    float operator[](unsigned int i) const;

    Vector2d operator+(const Vector2d& other) const;
    Vector2d& operator+=(const Vector2d& other);

    Vector2d operator-(const Vector2d& other) const;
    Vector2d& operator-=(const Vector2d& other);

    Vector2d operator*(const Vector2d& other) const;
    Vector2d& operator*=(const Vector2d& other);

    Vector2d operator/(const Vector2d& other) const;
    Vector2d& operator/=(const Vector2d& other);

    static float CrossProduct(const Vector2d& a, const Vector2d& b);
    static float DotProduct(const Vector2d& a, const Vector2d& b);

    Vector2d operator+(const float scalar) const;
    Vector2d& operator+=(const float scalar);

    Vector2d operator-(const float scalar) const;
    Vector2d& operator-=(const float scalar);

    Vector2d operator*(const float scalar) const;
    Vector2d& operator*=(const float scalar);

    Vector2d operator/(const float scalar) const;
    Vector2d& operator/=(const float scalar);

    bool operator<(const Vector2d& other) const;
    bool operator>(const Vector2d& other) const;
    bool operator<=(const Vector2d& other) const;
    bool operator>=(const Vector2d& other) const;
    bool operator==(const Vector2d& other) const;
    bool operator!=(const Vector2d& other) const;

    bool operator<(const float scalar) const;
    bool operator>(const float scalar) const;
    bool operator<=(const float scalar) const;
    bool operator>=(const float scalar) const;
    bool operator==(const float scalar) const;
    bool operator!=(const float scalar) const;

    Vector2d operator-() const;
    Vector2d operator+() const;

    Vector2d Perpendicular();
    Vector2d& Normalize();
    float Length();
    float Distance(const Vector2d& other);
    float DistanceSqrare(const Vector2d& other);

    static Vector2d Absolute(const Vector2d& other);

    static Vector2d Lerp(const Vector2d& a, const Vector2d& b, float t);  // Linear Interpolation
    static Vector2d Slerp(const Vector2d& a, const Vector2d& b, float t);  // Sphere Linear Interpolation
    static Vector2d Nlerp(const Vector2d& a, const Vector2d& b, float t);  // Normalized Linear Interpolation
    static Vector2d Normalize(const Vector2d& vector);

    float x, y;
    static const Vector2d up, down, left, right, one, zero;

  private:
    static void Swap(Vector2d& fisrt, Vector2d& second);
  };
}  // namespace Theodore

#endif /* Vector2d_h */
