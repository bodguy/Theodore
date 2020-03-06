// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Vector3d_h
#define Vector3d_h

#include "Vector2d.h"
#include "Vector4d.h"

namespace Theodore {
  class Vector3d {
  public:
    Vector3d();
    Vector3d(const float tx, const float ty, const float tz);
    explicit Vector3d(const Vector2d& other);
    Vector3d(const Vector2d& other, float tz);
    Vector3d(const float tx, const float ty);
    Vector3d(const Vector3d& other);
    explicit Vector3d(const Vector4d& other);
    Vector3d& operator=(Vector3d other);
    float operator[](unsigned int i) const;

    Vector3d operator+(const Vector3d& other) const;
    Vector3d& operator+=(const Vector3d& other);

    Vector3d operator-(const Vector3d& other) const;
    Vector3d& operator-=(const Vector3d& other);

    Vector3d operator*(const Vector3d& other) const;
    Vector3d& operator*=(const Vector3d& other);

    Vector3d operator/(const Vector3d& other) const;
    Vector3d& operator/=(const Vector3d& other);

    static Vector3d CrossProduct(const Vector3d& a, const Vector3d& b);
    static float DotProduct(const Vector3d& a, const Vector3d& b);

    Vector3d operator+(const float scalar) const;
    Vector3d& operator+=(const float scalar);

    Vector3d operator-(const float scalar) const;
    Vector3d& operator-=(const float scalar);

    Vector3d operator*(const float scalar) const;
    Vector3d& operator*=(const float scalar);

    Vector3d operator/(const float scalar) const;
    Vector3d& operator/=(const float scalar);

    bool operator<(const Vector3d& other) const;
    bool operator>(const Vector3d& other) const;
    bool operator<=(const Vector3d& other) const;
    bool operator>=(const Vector3d& other) const;
    bool operator==(const Vector3d& other) const;
    bool operator!=(const Vector3d& other) const;

    bool operator<(const float scalar) const;
    bool operator>(const float scalar) const;
    bool operator<=(const float scalar) const;
    bool operator>=(const float scalar) const;
    bool operator==(const float scalar) const;
    bool operator!=(const float scalar) const;

    Vector3d operator-() const;
    Vector3d operator+() const;
    Vector3d& Negate();

    static Vector3d Inverse(const Vector3d& other);
    Vector3d Perpendicular();
    Vector3d& Normalize();
    float Length() const;
    float SquaredLength() const;
    float Distance(const Vector3d& other) const;
    float DistanceSquare(const Vector3d& other) const;

    Vector2d xx() const { return Vector2d(x, x); }
    Vector2d xy() const { return Vector2d(x, y); }
    Vector2d xz() const { return Vector2d(x, z); }
    Vector2d yx() const { return Vector2d(y, x); }
    Vector2d yy() const { return Vector2d(y, y); }
    Vector2d yz() const { return Vector2d(y, z); }
    Vector2d zx() const { return Vector2d(z, x); }
    Vector2d zy() const { return Vector2d(z, y); }
    Vector2d zz() const { return Vector2d(z, z); }

    Vector3d xxx() const { return Vector3d(x, x, x); }
    Vector3d xxy() const { return Vector3d(x, x, y); }
    Vector3d xxz() const { return Vector3d(x, x, z); }
    Vector3d xyx() const { return Vector3d(x, y, x); }
    Vector3d xyy() const { return Vector3d(x, y, y); }
    Vector3d xyz() const { return Vector3d(x, y, z); }
    Vector3d xzx() const { return Vector3d(x, z, x); }
    Vector3d xzy() const { return Vector3d(x, z, y); }
    Vector3d xzz() const { return Vector3d(x, z, z); }

    Vector3d yxx() const { return Vector3d(y, x, x); }
    Vector3d yxy() const { return Vector3d(y, x, y); }
    Vector3d yxz() const { return Vector3d(y, x, z); }
    Vector3d yyx() const { return Vector3d(y, y, x); }
    Vector3d yyy() const { return Vector3d(y, y, y); }
    Vector3d yyz() const { return Vector3d(y, y, z); }
    Vector3d yzx() const { return Vector3d(y, z, x); }
    Vector3d yzy() const { return Vector3d(y, z, y); }
    Vector3d yzz() const { return Vector3d(y, z, z); }

    Vector3d zxx() const { return Vector3d(z, x, x); }
    Vector3d zxy() const { return Vector3d(z, x, y); }
    Vector3d zxz() const { return Vector3d(z, x, z); }
    Vector3d zyx() const { return Vector3d(z, y, x); }
    Vector3d zyy() const { return Vector3d(z, y, y); }
    Vector3d zyz() const { return Vector3d(z, y, z); }
    Vector3d zzx() const { return Vector3d(z, z, x); }
    Vector3d zzy() const { return Vector3d(z, z, y); }
    Vector3d zzz() const { return Vector3d(z, z, z); }

    static Vector2d ToVector2d(const Vector3d& other);
    static Vector4d ToVector4d(const Vector3d& other);
    static Vector3d Absolute(const Vector3d& other);

    static Vector3d Lerp(const Vector3d& a, const Vector3d& b, float t);  // Linear Interpolation
    static Vector3d Slerp(const Vector3d& a, const Vector3d& b, float t);  // Sphere Linear Interpolation
    static Vector3d Nlerp(const Vector3d& a, const Vector3d& b, float t);  // Normalized Linear Interpolation
    static Vector3d Project(const Vector3d& vector, const Vector3d& onNormal);
    static Vector3d ProjectOnPlane(const Vector3d& vector, const Vector3d& planeNormal);
    static Vector3d Normalize(const Vector3d& vector);
    static float Angle(const Vector3d& from, const Vector3d& to);

    float x, y, z;
    static const Vector3d up, down, left, right, forward, backward, one, zero;

  private:
    static void Swap(Vector3d& first, Vector3d& second);
  };
}  // namespace Theodore

#endif /* Vector3d_h */
