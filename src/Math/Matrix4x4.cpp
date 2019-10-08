// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Matrix4x4.h"
#include "Math.h"
#include "Quaternion.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include <algorithm>  // until c++11 for std::swap
#include <cmath>      // for std::sin, std::cos
#include <stdexcept>  // for std::logic_error, std::out_of_range
#include <utility>    // since c++11 for std::swap

namespace Theodore {
  Matrix4x4::Matrix4x4() { MakeIdentity(); }

  Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) {
    m16[0] = m11;
    m16[1] = m12;
    m16[2] = m13;
    m16[3] = m14;
    m16[4] = m21;
    m16[5] = m22;
    m16[6] = m23;
    m16[7] = m24;
    m16[8] = m31;
    m16[9] = m32;
    m16[10] = m33;
    m16[11] = m34;
    m16[12] = m41;
    m16[13] = m42;
    m16[14] = m43;
    m16[15] = m44;
  }

  Matrix4x4::Matrix4x4(const Matrix4x4& other) {
    for (int i = 0; i < 16; i++) {
      m16[i] = other.m16[i];
    }
  }

  Matrix4x4& Matrix4x4::operator=(Matrix4x4 other) {
    // copy and swap idiom (effective c++ section 11)
    Swap(*this, other);
    return *this;
  }

  bool Matrix4x4::operator==(const Matrix4x4& other) const {
    for (int i = 0; i < 16; i++) {
      if (m16[i] != other.m16[i]) return false;
    }

    return true;
  }

  bool Matrix4x4::operator!=(const Matrix4x4& other) const { return !(*this == other); }

  Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const {
    // using op= (more effective c++ section 22)
    return Matrix4x4(*this) += other;
  }

  Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {
    for (int i = 0; i < 16; i++) {
      m16[i] += other.m16[i];
    }

    return *this;
  }

  Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const { return Matrix4x4(*this) -= other; }

  Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
    for (int i = 0; i < 16; i++) {
      m16[i] -= other.m16[i];
    }

    return *this;
  }

  Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const { return Matrix4x4(*this) *= other; }

  Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
    /*
     A = | a  b |
         | c  d |
     B = | e  f |
         | g  h |
     A*B = | a  b || e  f | = | ae + bg  af+bh |
           | c  d || g  h |   | ce + dg  cf+dh |
    */
    Matrix4x4 result;
    result.m16[0] = m16[0] * other.m16[0] + m16[1] * other.m16[4] + m16[2] * other.m16[8] + m16[3] * other.m16[12];
    result.m16[1] = m16[0] * other.m16[1] + m16[1] * other.m16[5] + m16[2] * other.m16[9] + m16[3] * other.m16[13];
    result.m16[2] = m16[0] * other.m16[2] + m16[1] * other.m16[6] + m16[2] * other.m16[10] + m16[3] * other.m16[14];
    result.m16[3] = m16[0] * other.m16[3] + m16[1] * other.m16[7] + m16[2] * other.m16[11] + m16[3] * other.m16[15];

    result.m16[4] = m16[4] * other.m16[0] + m16[5] * other.m16[4] + m16[6] * other.m16[8] + m16[7] * other.m16[12];
    result.m16[5] = m16[4] * other.m16[1] + m16[5] * other.m16[5] + m16[6] * other.m16[9] + m16[7] * other.m16[13];
    result.m16[6] = m16[4] * other.m16[2] + m16[5] * other.m16[6] + m16[6] * other.m16[10] + m16[7] * other.m16[14];
    result.m16[7] = m16[4] * other.m16[3] + m16[5] * other.m16[7] + m16[6] * other.m16[11] + m16[7] * other.m16[15];

    result.m16[8] = m16[8] * other.m16[0] + m16[9] * other.m16[4] + m16[10] * other.m16[8] + m16[11] * other.m16[12];
    result.m16[9] = m16[8] * other.m16[1] + m16[9] * other.m16[5] + m16[10] * other.m16[9] + m16[11] * other.m16[13];
    result.m16[10] = m16[8] * other.m16[2] + m16[9] * other.m16[6] + m16[10] * other.m16[10] + m16[11] * other.m16[14];
    result.m16[11] = m16[8] * other.m16[3] + m16[9] * other.m16[7] + m16[10] * other.m16[11] + m16[11] * other.m16[15];

    result.m16[12] = m16[12] * other.m16[0] + m16[13] * other.m16[4] + m16[14] * other.m16[8] + m16[15] * other.m16[12];
    result.m16[13] = m16[12] * other.m16[1] + m16[13] * other.m16[5] + m16[14] * other.m16[9] + m16[15] * other.m16[13];
    result.m16[14] = m16[12] * other.m16[2] + m16[13] * other.m16[6] + m16[14] * other.m16[10] + m16[15] * other.m16[14];
    result.m16[15] = m16[12] * other.m16[3] + m16[13] * other.m16[7] + m16[14] * other.m16[11] + m16[15] * other.m16[15];

    *this = result;
    return *this;
  }

  Matrix4x4 Matrix4x4::operator*(const float scalar) const { return Matrix4x4(*this) *= scalar; }

  Matrix4x4& Matrix4x4::operator*=(const float scalar) {
    for (int i = 0; i < 16; i++) {
      m16[i] *= scalar;
    }

    return *this;
  }

  Matrix4x4 Matrix4x4::operator/(const float scalar) const { return Matrix4x4(*this) /= scalar; }

  Matrix4x4& Matrix4x4::operator/=(const float scalar) {
    for (int i = 0; i < 16; i++) {
      m16[i] /= scalar;
    }

    return *this;
  }

  Vector4d Matrix4x4::operator*(const Vector4d& other) {
    return Vector4d(m16[0] * other.x + m16[1] * other.y + m16[2] * other.z + m16[3] * other.w, m16[4] * other.x + m16[5] * other.y + m16[6] * other.z + m16[7] * other.w,
                    m16[8] * other.x + m16[9] * other.y + m16[10] * other.z + m16[11] * other.w, m16[12] * other.x + m16[13] * other.y + m16[14] * other.z + m16[15] * other.w);
  }

  const float Matrix4x4::operator()(unsigned int row, unsigned int col) const { return m16[row * 4 + col]; }

  const float Matrix4x4::operator[](unsigned int index) const { return m16[index]; }

  const float Matrix4x4::At(int index) const {
    if (index >= 0 && index <= 16) {
      return (*this)[index];
    }

    throw std::out_of_range("index is out of range!");
  }

  const float Matrix4x4::At(int row, int col) const {
    if (row >= 0 && row <= 3 && col >= 0 && col <= 3) {
      return (*this).m44[row][col];
    }

    throw std::out_of_range("index is out of range!");
  }

  Matrix4x4& Matrix4x4::MakeIdentity() { return *this = Identity(); }

  Matrix4x4 Matrix4x4::Identity() { return Matrix4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f); }

  bool Matrix4x4::IsIdentity() const { return *this == Identity(); }

  Matrix4x4& Matrix4x4::MakeInverse() {
    // 		   | a  b |
    //   A = | c  d |

    //	       	  1  | d  -b |
    //   A^-1 = ad-bc| -c  a |
    float determinant = m16[0] * m16[5] * m16[10] * m16[15] + m16[0] * m16[6] * m16[11] * m16[13] + m16[0] * m16[7] * m16[9] * m16[14] + m16[1] * m16[4] * m16[11] * m16[14] +
                        m16[1] * m16[6] * m16[8] * m16[15] + m16[1] * m16[7] * m16[10] * m16[12] + m16[2] * m16[4] * m16[9] * m16[15] + m16[2] * m16[5] * m16[11] * m16[12] +
                        m16[2] * m16[7] * m16[8] * m16[13] + m16[3] * m16[4] * m16[10] * m16[13] + m16[3] * m16[5] * m16[8] * m16[14] + m16[3] * m16[6] * m16[9] * m16[12] -
                        m16[0] * m16[5] * m16[11] * m16[14] - m16[0] * m16[6] * m16[9] * m16[15] - m16[0] * m16[7] * m16[10] * m16[13] - m16[1] * m16[4] * m16[10] * m16[15] -
                        m16[1] * m16[6] * m16[11] * m16[12] - m16[1] * m16[7] * m16[8] * m16[14] - m16[2] * m16[4] * m16[11] * m16[13] - m16[2] * m16[5] * m16[8] * m16[15] -
                        m16[2] * m16[7] * m16[9] * m16[12] - m16[3] * m16[4] * m16[9] * m16[14] - m16[3] * m16[5] * m16[10] * m16[12] - m16[3] * m16[6] * m16[8] * m16[13];

    if (determinant == 0) throw std::logic_error("determinant is zero! Inverse does not exist.");

    Matrix4x4 mat = Matrix4x4(m16[5] * m16[10] * m16[15] + m16[6] * m16[11] * m16[13] + m16[7] * m16[9] * m16[14] - m16[5] * m16[11] * m16[14] - m16[6] * m16[9] * m16[15] - m16[7] * m16[10] * m16[13],
                              m16[1] * m16[11] * m16[14] + m16[2] * m16[9] * m16[15] + m16[3] * m16[10] * m16[13] - m16[1] * m16[10] * m16[15] - m16[2] * m16[11] * m16[13] - m16[3] * m16[9] * m16[14],
                              m16[1] * m16[6] * m16[15] + m16[2] * m16[7] * m16[13] + m16[3] * m16[5] * m16[14] - m16[1] * m16[7] * m16[14] - m16[2] * m16[5] * m16[15] - m16[3] * m16[6] * m16[13],
                              m16[1] * m16[7] * m16[10] + m16[2] * m16[5] * m16[11] + m16[3] * m16[6] * m16[9] - m16[1] * m16[6] * m16[11] - m16[2] * m16[7] * m16[9] - m16[3] * m16[5] * m16[10],
                              m16[4] * m16[11] * m16[14] + m16[6] * m16[8] * m16[15] + m16[7] * m16[10] * m16[12] - m16[4] * m16[10] * m16[15] - m16[6] * m16[11] * m16[12] - m16[7] * m16[8] * m16[14],
                              m16[0] * m16[10] * m16[15] + m16[2] * m16[11] * m16[12] + m16[3] * m16[8] * m16[14] - m16[0] * m16[11] * m16[14] - m16[2] * m16[8] * m16[15] - m16[3] * m16[10] * m16[12],
                              m16[0] * m16[7] * m16[14] + m16[2] * m16[4] * m16[15] + m16[3] * m16[6] * m16[12] - m16[0] * m16[6] * m16[15] - m16[2] * m16[7] * m16[12] - m16[3] * m16[4] * m16[14],
                              m16[0] * m16[6] * m16[11] + m16[2] * m16[7] * m16[8] + m16[3] * m16[4] * m16[10] - m16[0] * m16[7] * m16[10] - m16[2] * m16[4] * m16[11] - m16[3] * m16[6] * m16[8],
                              m16[4] * m16[9] * m16[15] + m16[5] * m16[11] * m16[12] + m16[7] * m16[8] * m16[13] - m16[4] * m16[11] * m16[13] - m16[5] * m16[8] * m16[15] - m16[7] * m16[9] * m16[12],
                              m16[0] * m16[11] * m16[13] + m16[1] * m16[8] * m16[15] + m16[3] * m16[9] * m16[12] - m16[0] * m16[9] * m16[15] - m16[1] * m16[11] * m16[12] - m16[3] * m16[8] * m16[13],
                              m16[0] * m16[5] * m16[15] + m16[1] * m16[7] * m16[12] + m16[3] * m16[4] * m16[13] - m16[0] * m16[7] * m16[13] - m16[1] * m16[4] * m16[15] - m16[3] * m16[5] * m16[12],
                              m16[0] * m16[7] * m16[9] + m16[1] * m16[4] * m16[11] + m16[3] * m16[5] * m16[8] - m16[0] * m16[5] * m16[11] - m16[1] * m16[7] * m16[8] - m16[3] * m16[4] * m16[9],
                              m16[4] * m16[10] * m16[13] + m16[5] * m16[8] * m16[14] + m16[6] * m16[9] * m16[12] - m16[4] * m16[9] * m16[14] - m16[5] * m16[10] * m16[12] - m16[6] * m16[8] * m16[13],
                              m16[0] * m16[9] * m16[14] + m16[1] * m16[10] * m16[12] + m16[2] * m16[8] * m16[13] - m16[0] * m16[10] * m16[13] - m16[1] * m16[8] * m16[14] - m16[2] * m16[9] * m16[12],
                              m16[0] * m16[6] * m16[13] + m16[1] * m16[4] * m16[14] + m16[2] * m16[5] * m16[12] - m16[0] * m16[5] * m16[14] - m16[1] * m16[6] * m16[12] - m16[2] * m16[4] * m16[13],
                              m16[0] * m16[5] * m16[10] + m16[1] * m16[6] * m16[8] + m16[2] * m16[4] * m16[9] - m16[0] * m16[6] * m16[9] - m16[1] * m16[4] * m16[10] - m16[2] * m16[5] * m16[8]);

    *this = mat * (1 / determinant);
    return *this;
  }

  Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& other) {
    float determinant =
        other.m16[0] * other.m16[5] * other.m16[10] * other.m16[15] + other.m16[0] * other.m16[6] * other.m16[11] * other.m16[13] + other.m16[0] * other.m16[7] * other.m16[9] * other.m16[14] +
        other.m16[1] * other.m16[4] * other.m16[11] * other.m16[14] + other.m16[1] * other.m16[6] * other.m16[8] * other.m16[15] + other.m16[1] * other.m16[7] * other.m16[10] * other.m16[12] +
        other.m16[2] * other.m16[4] * other.m16[9] * other.m16[15] + other.m16[2] * other.m16[5] * other.m16[11] * other.m16[12] + other.m16[2] * other.m16[7] * other.m16[8] * other.m16[13] +
        other.m16[3] * other.m16[4] * other.m16[10] * other.m16[13] + other.m16[3] * other.m16[5] * other.m16[8] * other.m16[14] + other.m16[3] * other.m16[6] * other.m16[9] * other.m16[12] -
        other.m16[0] * other.m16[5] * other.m16[11] * other.m16[14] - other.m16[0] * other.m16[6] * other.m16[9] * other.m16[15] - other.m16[0] * other.m16[7] * other.m16[10] * other.m16[13] -
        other.m16[1] * other.m16[4] * other.m16[10] * other.m16[15] - other.m16[1] * other.m16[6] * other.m16[11] * other.m16[12] - other.m16[1] * other.m16[7] * other.m16[8] * other.m16[14] -
        other.m16[2] * other.m16[4] * other.m16[11] * other.m16[13] - other.m16[2] * other.m16[5] * other.m16[8] * other.m16[15] - other.m16[2] * other.m16[7] * other.m16[9] * other.m16[12] -
        other.m16[3] * other.m16[4] * other.m16[9] * other.m16[14] - other.m16[3] * other.m16[5] * other.m16[10] * other.m16[12] - other.m16[3] * other.m16[6] * other.m16[8] * other.m16[13];

    if (determinant == 0) throw std::logic_error("determinant is zero! Inverse does not exist.");

    Matrix4x4 mat = Matrix4x4(other.m16[5] * other.m16[10] * other.m16[15] + other.m16[6] * other.m16[11] * other.m16[13] + other.m16[7] * other.m16[9] * other.m16[14] -
                                  other.m16[5] * other.m16[11] * other.m16[14] - other.m16[6] * other.m16[9] * other.m16[15] - other.m16[7] * other.m16[10] * other.m16[13],
                              other.m16[1] * other.m16[11] * other.m16[14] + other.m16[2] * other.m16[9] * other.m16[15] + other.m16[3] * other.m16[10] * other.m16[13] -
                                  other.m16[1] * other.m16[10] * other.m16[15] - other.m16[2] * other.m16[11] * other.m16[13] - other.m16[3] * other.m16[9] * other.m16[14],
                              other.m16[1] * other.m16[6] * other.m16[15] + other.m16[2] * other.m16[7] * other.m16[13] + other.m16[3] * other.m16[5] * other.m16[14] -
                                  other.m16[1] * other.m16[7] * other.m16[14] - other.m16[2] * other.m16[5] * other.m16[15] - other.m16[3] * other.m16[6] * other.m16[13],
                              other.m16[1] * other.m16[7] * other.m16[10] + other.m16[2] * other.m16[5] * other.m16[11] + other.m16[3] * other.m16[6] * other.m16[9] -
                                  other.m16[1] * other.m16[6] * other.m16[11] - other.m16[2] * other.m16[7] * other.m16[9] - other.m16[3] * other.m16[5] * other.m16[10],
                              other.m16[4] * other.m16[11] * other.m16[14] + other.m16[6] * other.m16[8] * other.m16[15] + other.m16[7] * other.m16[10] * other.m16[12] -
                                  other.m16[4] * other.m16[10] * other.m16[15] - other.m16[6] * other.m16[11] * other.m16[12] - other.m16[7] * other.m16[8] * other.m16[14],
                              other.m16[0] * other.m16[10] * other.m16[15] + other.m16[2] * other.m16[11] * other.m16[12] + other.m16[3] * other.m16[8] * other.m16[14] -
                                  other.m16[0] * other.m16[11] * other.m16[14] - other.m16[2] * other.m16[8] * other.m16[15] - other.m16[3] * other.m16[10] * other.m16[12],
                              other.m16[0] * other.m16[7] * other.m16[14] + other.m16[2] * other.m16[4] * other.m16[15] + other.m16[3] * other.m16[6] * other.m16[12] -
                                  other.m16[0] * other.m16[6] * other.m16[15] - other.m16[2] * other.m16[7] * other.m16[12] - other.m16[3] * other.m16[4] * other.m16[14],
                              other.m16[0] * other.m16[6] * other.m16[11] + other.m16[2] * other.m16[7] * other.m16[8] + other.m16[3] * other.m16[4] * other.m16[10] -
                                  other.m16[0] * other.m16[7] * other.m16[10] - other.m16[2] * other.m16[4] * other.m16[11] - other.m16[3] * other.m16[6] * other.m16[8],
                              other.m16[4] * other.m16[9] * other.m16[15] + other.m16[5] * other.m16[11] * other.m16[12] + other.m16[7] * other.m16[8] * other.m16[13] -
                                  other.m16[4] * other.m16[11] * other.m16[13] - other.m16[5] * other.m16[8] * other.m16[15] - other.m16[7] * other.m16[9] * other.m16[12],
                              other.m16[0] * other.m16[11] * other.m16[13] + other.m16[1] * other.m16[8] * other.m16[15] + other.m16[3] * other.m16[9] * other.m16[12] -
                                  other.m16[0] * other.m16[9] * other.m16[15] - other.m16[1] * other.m16[11] * other.m16[12] - other.m16[3] * other.m16[8] * other.m16[13],
                              other.m16[0] * other.m16[5] * other.m16[15] + other.m16[1] * other.m16[7] * other.m16[12] + other.m16[3] * other.m16[4] * other.m16[13] -
                                  other.m16[0] * other.m16[7] * other.m16[13] - other.m16[1] * other.m16[4] * other.m16[15] - other.m16[3] * other.m16[5] * other.m16[12],
                              other.m16[0] * other.m16[7] * other.m16[9] + other.m16[1] * other.m16[4] * other.m16[11] + other.m16[3] * other.m16[5] * other.m16[8] -
                                  other.m16[0] * other.m16[5] * other.m16[11] - other.m16[1] * other.m16[7] * other.m16[8] - other.m16[3] * other.m16[4] * other.m16[9],
                              other.m16[4] * other.m16[10] * other.m16[13] + other.m16[5] * other.m16[8] * other.m16[14] + other.m16[6] * other.m16[9] * other.m16[12] -
                                  other.m16[4] * other.m16[9] * other.m16[14] - other.m16[5] * other.m16[10] * other.m16[12] - other.m16[6] * other.m16[8] * other.m16[13],
                              other.m16[0] * other.m16[9] * other.m16[14] + other.m16[1] * other.m16[10] * other.m16[12] + other.m16[2] * other.m16[8] * other.m16[13] -
                                  other.m16[0] * other.m16[10] * other.m16[13] - other.m16[1] * other.m16[8] * other.m16[14] - other.m16[2] * other.m16[9] * other.m16[12],
                              other.m16[0] * other.m16[6] * other.m16[13] + other.m16[1] * other.m16[4] * other.m16[14] + other.m16[2] * other.m16[5] * other.m16[12] -
                                  other.m16[0] * other.m16[5] * other.m16[14] - other.m16[1] * other.m16[6] * other.m16[12] - other.m16[2] * other.m16[4] * other.m16[13],
                              other.m16[0] * other.m16[5] * other.m16[10] + other.m16[1] * other.m16[6] * other.m16[8] + other.m16[2] * other.m16[4] * other.m16[9] -
                                  other.m16[0] * other.m16[6] * other.m16[9] - other.m16[1] * other.m16[4] * other.m16[10] - other.m16[2] * other.m16[5] * other.m16[8]);

    Matrix4x4 result = mat * (1 / determinant);
    return result;
  }

  Matrix4x4& Matrix4x4::MakeTranspose() {
    //     | a  b |
    // A = | c  d |
    //      | a  c |
    // At = | b  d |
    std::swap(m16[1], m16[4]);
    std::swap(m16[2], m16[8]);
    std::swap(m16[3], m16[12]);
    std::swap(m16[6], m16[9]);
    std::swap(m16[7], m16[13]);
    std::swap(m16[11], m16[14]);

    return *this;
  }

  const float* Matrix4x4::Pointer() const { return m16; }

  Matrix4x4 Matrix4x4::Perspective(float fieldOfView, float aspectRatio, float znear, float zfar) {
    float tanHalfFovy = std::tan(fieldOfView / 2.f);

    Matrix4x4 mat = Matrix4x4::Zero();
    mat.m16[0] = 1.0f / (aspectRatio * tanHalfFovy);
    mat.m16[5] = 1.0f / tanHalfFovy;
    mat.m16[10] = -(zfar + znear) / (zfar - znear);
    mat.m16[11] = -1.0f;
    mat.m16[14] = -(2.0f * zfar * znear) / (zfar - znear);

    return mat;
  }

  Matrix4x4 Matrix4x4::Orthogonal(float left, float right, float bottom, float top, float znear, float zfar) {
    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = 1.0f / (zfar - znear);

    Matrix4x4 mat;
    mat.m16[0] = 2.f * rl;
    mat.m16[5] = 2.f * tb;
    mat.m16[10] = -2.f * fn;
    mat.m16[12] = -(right + left) * rl;
    mat.m16[13] = -(top + bottom) * tb;
    mat.m16[14] = -(zfar + znear) * fn;

    return mat;
  }

  Matrix4x4 Matrix4x4::LookAt(const Vector3d& eye, const Vector3d& lookat, const Vector3d& up) {
    Vector3d zaxis = (eye - lookat).Normalize();
    Vector3d xaxis = Vector3d::CrossProduct(up, zaxis).Normalize();
    Vector3d yaxis = Vector3d::CrossProduct(zaxis, xaxis);

    return Matrix4x4(xaxis.x, yaxis.x, zaxis.x, 0.f, xaxis.y, yaxis.y, zaxis.y, 0.f, xaxis.z, yaxis.z, zaxis.z, 0.f, -Vector3d::DotProduct(xaxis, eye), -Vector3d::DotProduct(yaxis, eye),
                     -Vector3d::DotProduct(zaxis, eye), 1.f);
  }

  Matrix4x4 Matrix4x4::Translate(const Vector3d& translation) {
    /*
     |1 0 0 0|
     |0 1 0 0|
     |0 0 1 0|
     |x y z 1|
    */
    Matrix4x4 mat;
    mat.m16[12] = translation.x;
    mat.m16[13] = translation.y;
    mat.m16[14] = translation.z;

    return mat;
  }

  Matrix4x4 Matrix4x4::Scale(const Vector3d& scale) {
    /*
     |x 0 0 0|
     |0 y 0 0|
     |0 0 z 0|
     |0 0 0 1|
    */
    Matrix4x4 mat;
    mat.m16[0] = scale.x;
    mat.m16[5] = scale.y;
    mat.m16[10] = scale.z;

    return mat;
  }

  Matrix4x4 Matrix4x4::Rotation(float radianAngle, const Vector3d& axis) {
    /*
     x = | 1  0    0  0 |
     | 0 cos -sin 0 |
     | 0 sin  cos 0 |
     | 0  0    0  1 |

     y = |  cos  0  sin  0 |
     |   0	1	0   0 |
     | -sin  0  cos	0 |
     |   0	0	0   1 |

     z = | cos -sin 0 0 |
     | sin  cos 0 0 |
     |  0    0  1 0 |
     |  0    0  0 1 |
    */
    float c = std::cos(radianAngle);
    float s = std::sin(radianAngle);
    float t = 1.f - c;

    float tx = t * axis.x;
    float ty = t * axis.y;
    float tz = t * axis.z;

    float sx = s * axis.x;
    float sy = s * axis.y;
    float sz = s * axis.z;

    return Matrix4x4(tx * axis.x + c, tx * axis.y + sz, tx * axis.z - sy, 0.f, ty * axis.x - sz, ty * axis.y + c, ty * axis.z + sx, 0.f, tz * axis.x + sy, tz * axis.y - sx, tz * axis.z + c, 0.f, 0.f,
                     0.f, 0.f, 1.f);
  }

  Matrix4x4 Matrix4x4::Zero() { return Matrix4x4(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f); }

  Matrix4x4 Matrix4x4::ToMatrix3x3(const Matrix4x4& other) {
    return Matrix4x4(other.m16[0], other.m16[1], other.m16[2], other.m16[3], other.m16[4], other.m16[5], other.m16[6], other.m16[7], other.m16[8], other.m16[9], other.m16[10], other.m16[11], 0.f, 0.f,
                     0.f, 1.f);
  }

  Matrix4x4 Matrix4x4::Absolute(const Matrix4x4& other) {
    return Matrix4x4(std::fabsf(other.m16[0]), std::fabsf(other.m16[1]), std::fabsf(other.m16[2]), std::fabsf(other.m16[3]), std::fabsf(other.m16[4]), std::fabsf(other.m16[5]),
                     std::fabsf(other.m16[6]), std::fabsf(other.m16[7]), std::fabsf(other.m16[8]), std::fabsf(other.m16[9]), std::fabsf(other.m16[10]), std::fabsf(other.m16[11]),
                     std::fabsf(other.m16[12]), std::fabsf(other.m16[13]), std::fabsf(other.m16[14]), std::fabsf(other.m16[15]));
  }

  Matrix4x4 Matrix4x4::OrthoNormalize(const Matrix4x4& matrix) {
    Vector4d first = Vector4d(matrix._11, matrix._12, matrix._13, matrix._14).Normalize();
    Vector4d second = Vector4d(matrix._21, matrix._22, matrix._23, matrix._24).Normalize();
    Vector4d third = Vector4d(matrix._31, matrix._32, matrix._33, matrix._34).Normalize();

    return Matrix4x4(first.x, first.y, first.z, first.w, second.x, second.y, second.z, second.w, third.x, third.y, third.z, third.w, matrix._41, matrix._42, matrix._43, matrix._44);
  }

  Vector3d Matrix4x4::DecomposeTranslation(const Matrix4x4& transformation) { return Vector3d(transformation._41, transformation._42, transformation._43); }

  Vector3d Matrix4x4::DecomposeRotation(const Matrix4x4& transformation) {
    Matrix4x4 normalized = Matrix4x4::OrthoNormalize(transformation);

    // In radian unit
    return Vector3d(std::atan2f(normalized._23, normalized._33),
                    -std::asinf(normalized._13),  // std::atan2f(-normalized._13,
                                                  // std::sqrtf(normalized._23 * normalized._23 +
                                                  // normalized._33 * normalized._33)),
                    std::atan2f(normalized._12, normalized._11));
  }

  Vector3d Matrix4x4::DecomposeScale(const Matrix4x4& transformation) {
    float first = Vector3d(transformation._11, transformation._12, transformation._13).Length();
    float second = Vector3d(transformation._21, transformation._22, transformation._23).Length();
    float third = Vector3d(transformation._31, transformation._32, transformation._33).Length();

    return Vector3d(first, second, third);
  }

  Matrix4x4 Matrix4x4::TRS(const Vector3d& p, const Quaternion& q, const Vector3d& s) {
    /*
      If using column vectors
      M := T * R * S
      else if using row vectors
      M := S * R * T
    */
    return Matrix4x4::Scale(s) * Quaternion::ToRotationMatrix(q) * Matrix4x4::Translate(p);
  }

  void Matrix4x4::Swap(Matrix4x4& first, Matrix4x4& second) {
    using std::swap;
    for (int i = 0; i < 16; i++) {
      swap(first.m16[i], second.m16[i]);
    }
  }
}  // namespace Theodore
