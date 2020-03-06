// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Matrix4x4_h
#define Matrix4x4_h

#include "Vector4d.h"

namespace Theodore {
  class Vector2d;
  class Vector3d;
  class Vector4d;
  class Quaternion;
  class Matrix4x4 {
  public:
    Matrix4x4();
    Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    Matrix4x4(const Matrix4x4& other);
    Matrix4x4& operator=(Matrix4x4 other);

    bool operator==(const Matrix4x4& other) const;
    bool operator!=(const Matrix4x4& other) const;

    Matrix4x4 operator+(const Matrix4x4& other) const;
    Matrix4x4& operator+=(const Matrix4x4& other);

    Matrix4x4 operator-(const Matrix4x4& other) const;
    Matrix4x4& operator-=(const Matrix4x4& other);

    Matrix4x4 operator*(const Matrix4x4& other) const;
    Matrix4x4& operator*=(const Matrix4x4& other);

    Matrix4x4 operator*(const float scalar) const;
    Matrix4x4& operator*=(const float scalar);

    Matrix4x4 operator/(const float scalar) const;
    Matrix4x4& operator/=(const float scalar);

    Vector4d operator*(const Vector4d& other);

    const float operator()(unsigned int row, unsigned int col) const;
    const float operator[](unsigned int index) const;
    const float At(int index) const;
    const float At(int row, int col) const;

    Matrix4x4& MakeIdentity();
    static Matrix4x4 Identity();
    bool IsIdentity() const;

    Matrix4x4& MakeInverse();
    static Matrix4x4 Inverse(const Matrix4x4& other);

    Matrix4x4& MakeTranspose();

    //! get native matrix pointer, Note : this is for shader
    const float* Pointer() const;

    static Matrix4x4 Perspective(float fieldOfView, float aspectRatio, float znear, float zfar);
    static Matrix4x4 Orthogonal(float left, float right, float bottom, float top, float znear, float zfar);
    static Matrix4x4 LookAt(const Vector3d& eye, const Vector3d& lookat, const Vector3d& up);
    static Matrix4x4 Translate(const Vector3d& translation);
    static Matrix4x4 Scale(const Vector3d& scale);
    static Matrix4x4 Rotation(float radianAngle, const Vector3d& axis);
    static Matrix4x4 Zero();
    static Matrix4x4 ToMatrix3x3(const Matrix4x4& other);
    static Matrix4x4 Absolute(const Matrix4x4& other);
    static Matrix4x4 OrthoNormalize(const Matrix4x4& matrix);
    static Vector3d DecomposeTranslation(const Matrix4x4& transformation);
    static Vector3d DecomposeRotation(const Matrix4x4& transformation);
    static Vector3d DecomposeScale(const Matrix4x4& transformation);
    static Matrix4x4 TRS(const Vector3d& p, const Quaternion& q, const Vector3d& s);

    // anonymous union
    union {
      struct {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
      };

      float m44[4][4];
      float m16[16];
      Vector4d rows[4];
    };

  private:
    static void Swap(Matrix4x4& first, Matrix4x4& second);
  };
}  // namespace Theodore

#endif /* Matrix4x4_h */
