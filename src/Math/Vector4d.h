// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Vector4d_h
#define Vector4d_h

namespace Theodore {
  class Vector2d;
  class Vector3d;
  class Vector4d {
  public:
    Vector4d();
    Vector4d(float tx, float ty, float tz, float tw);
    explicit Vector4d(const Vector2d& other);
    explicit Vector4d(const Vector3d& other);
    Vector4d(const Vector2d& other, float tz, float tw);
    Vector4d(const Vector3d& other, float tw);
    Vector4d(const Vector4d& other);
    Vector4d& operator=(Vector4d other);
    float operator[](unsigned int i) const;

    Vector4d operator+(const Vector4d& other) const;
    Vector4d& operator+=(const Vector4d& other);

    Vector4d operator-(const Vector4d& other) const;
    Vector4d& operator-=(const Vector4d& other);

    Vector4d operator*(const Vector4d& other) const;
    Vector4d& operator*=(const Vector4d& other);

    Vector4d operator/(const Vector4d& other) const;
    Vector4d& operator/=(const Vector4d& other);

    static float DotProduct(const Vector4d& a, const Vector4d& b);

    Vector4d operator+(const float scalar) const;
    Vector4d& operator+=(const float scalar);

    Vector4d operator-(const float scalar) const;
    Vector4d& operator-=(const float scalar);

    Vector4d operator*(const float scalar) const;
    Vector4d& operator*=(const float scalar);

    Vector4d operator/(const float scalar) const;
    Vector4d& operator/=(const float scalar);

    bool operator<(const Vector4d& other) const;
    bool operator>(const Vector4d& other) const;
    bool operator<=(const Vector4d& other) const;
    bool operator>=(const Vector4d& other) const;
    bool operator==(const Vector4d& other) const;
    bool operator!=(const Vector4d& other) const;

    bool operator<(const float scalar) const;
    bool operator>(const float scalar) const;
    bool operator<=(const float scalar) const;
    bool operator>=(const float scalar) const;
    bool operator==(const float scalar) const;
    bool operator!=(const float scalar) const;

    Vector4d& Normalize();
    float Length();
    static Vector3d ToVector3d(const Vector4d& other);
    static Vector4d Absolute(const Vector4d& other);

    float x, y, z, w;
    static const Vector4d up, down, left, right, forward, backward, one, zero;

  private:
    static void Swap(Vector4d& first, Vector4d& second);
  };
}  // namespace Theodore

#endif /* Vector4d_h */
