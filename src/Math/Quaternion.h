// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Quaternion_h
#define Quaternion_h

#include "Matrix4x4.h"

namespace Theodore {
  class Vector3d;
  class Quaternion {
  public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float w, const Vector3d& v);
    Quaternion(const Quaternion& other);
    const Quaternion& operator=(Quaternion other);

    Quaternion operator+(const Quaternion& other) const;
    Quaternion& operator+=(const Quaternion& other);

    Quaternion operator-(const Quaternion& other) const;
    Quaternion& operator-=(const Quaternion& other);

    Quaternion operator*(const Quaternion& other) const;
    Quaternion& operator*=(const Quaternion& other);
    Quaternion& operator*=(const Vector3d& other);
    Vector3d operator*(const Vector3d& other) const;

    Quaternion operator+(const float scalar) const;
    Quaternion& operator+=(const float scalar);

    Quaternion operator-(const float scalar) const;
    Quaternion& operator-=(const float scalar);

    Quaternion operator*(const float scalar) const;
    Quaternion& operator*=(const float scalar);

    Quaternion operator/(const float scalar) const;
    Quaternion& operator/=(const float scalar);

    bool operator<(const Quaternion& other) const;
    bool operator>(const Quaternion& other) const;
    bool operator<=(const Quaternion& other) const;
    bool operator>=(const Quaternion& other) const;
    bool operator==(const Quaternion& other) const;
    bool operator!=(const Quaternion& other) const;

    bool operator<(const float scalar) const;
    bool operator>(const float scalar) const;
    bool operator<=(const float scalar) const;
    bool operator>=(const float scalar) const;

    Quaternion& Conjugate();
    Quaternion& Normalize();
    Quaternion& MakeInverse();
    float Length() const;

    Vector3d ConjugateProduct(const Vector3d& v) const;
    static float DotProduct(const Quaternion& a, const Quaternion& b);
    static Quaternion Inverse(const Quaternion& other);
    static Vector3d ToEuler(const Quaternion& quat);
    static Quaternion FromEuler(const Vector3d& euler);
    void AxisAngle(Vector3d& axis, float& angle);
    static Quaternion AngleAxis(const float angle, const Vector3d& axis);
    static Matrix4x4 ToRotationMatrix(const Quaternion& other);
    static Quaternion FromRotationMatrix(const Matrix4x4& other);
    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

    // vector(imaginary) part, real part
    float x, y, z, w;

    static const Quaternion zero, identity;

  private:
    static void Swap(Quaternion& first, Quaternion& second);
  };
}  // namespace Theodore

#endif /* Quaternion_h */
