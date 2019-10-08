#include "Debug.h"
#include <stdarg.h>  // for va_list
#include <cstdio>   // for vprintf
#include "../Asset/Texture2D.h"
#include "../Geometry/Ray.h"
#include "../Math/Quaternion.h"
#include "../Platform/Time.h"

namespace Theodore {
  const size_t Debug::maxLength = 256;

  void Debug::Error(const char* format, ...) {
    char fmt[Debug::maxLength];
    va_list args;
    va_start(args, format);
    snprintf(fmt, Debug::maxLength, "%s [ERROR] %s\n", Time::GetDateTime().c_str(), format);
    vprintf(fmt, args);
    va_end(args);
  }

  void Debug::Warn(const char* format, ...) {
    char fmt[Debug::maxLength];
    va_list args;
    va_start(args, format);
    snprintf(fmt, Debug::maxLength, "%s [WARN] %s\n", Time::GetDateTime().c_str(), format);
    vprintf(fmt, args);
    va_end(args);
  }

  void Debug::Trace(const char* format, ...) {
    char fmt[Debug::maxLength];
    va_list args;
    va_start(args, format);
    snprintf(fmt, Debug::maxLength, "%s [TRACE] %s\n", Time::GetDateTime().c_str(), format);
    vprintf(fmt, args);
    va_end(args);
  }

  void Debug::Log(const char* format, ...) {
    char fmt[Debug::maxLength];
    va_list args;
    va_start(args, format);
    snprintf(fmt, Debug::maxLength, "%s [INFO] %s\n", Time::GetDateTime().c_str(), format);
    vprintf(fmt, args);
    va_end(args);
  }

  void Debug::Log(const std::string& str) { Debug::Log("%s", str.c_str()); }

  void Debug::Log(bool b) {
    const char* t_of_f = b ? "True" : "False";
    Debug::Log("%s", t_of_f);
  }

  void Debug::Log(char c) { Debug::Log("%c", c); }

  void Debug::Log(short s) { Debug::Log("%d", s); }

  void Debug::Log(unsigned short s) { Debug::Log("%hd", s); }

  void Debug::Log(int i) { Debug::Log("%d", i); }

  void Debug::Log(unsigned int ui) { Debug::Log("%u", ui); }

  void Debug::Log(long l) { Debug::Log("%l", l); }

  void Debug::Log(unsigned long ul) { Debug::Log("%lu", ul); }

  void Debug::Log(float f) { Debug::Log("%f", f); }

  void Debug::Log(double d) { Debug::Log("%lf", d); }

  void Debug::Log(const Matrix4x4& matrix, unsigned int precision) {
    static const char* precision_table[7] = {"%.0f ", "%.1f ", "%.2f ", "%.3f ", "%.4f ", "%.5f ", "%.6f "};
    Debug::Log("Matrix4x4 = ");

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        printf(precision_table[precision], matrix.m44[i][j]);
      }
      printf("\n");
    }
  }

  void Debug::Log(const Vector4d& vector4) { Debug::Log("Vector4d = x: %f, y: %f, z: %f, w: %f", vector4.x, vector4.y, vector4.z, vector4.w); }

  void Debug::Log(const Vector3d& vector3) { Debug::Log("Vector3d = x: %f, y: %f, z: %f", vector3.x, vector3.y, vector3.z); }

  void Debug::Log(const Vector2d& vector2) { Debug::Log("Vector2d = x: %f, y: %f", vector2.x, vector2.y); }

  void Debug::Log(const Color& color) {
    // clang-format off
    Debug::Log("Color = r:%f[%d] g:%f[%d] b:%f[%d] a:%f[%d]",
            color.r, Color::ConvertToByte(color.r),
            color.g, Color::ConvertToByte(color.g),
            color.b, Color::ConvertToByte(color.b),
            color.a, Color::ConvertToByte(color.a)
    );
    // clang-format on
  }

  void Debug::Log(const Quaternion& quat) { Debug::Log("Quaternion = w: %f, x: %f, y: %f, z: %f", quat.w, quat.x, quat.y, quat.z); }

  void Debug::Log(const Ray& ray) { Debug::Log("Origin: (%f, %f, %f), Dir: (%f, %f, %f)", ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z); }

  void Debug::Log(const Texture* texture) {
    // clang-format off
    Debug::Log("Texture loaded %s[w=%d, h=%d], ref=%d, id=%d format=%d",
               texture->mBaseName.c_str(), texture->mWidth, texture->mHeight, texture->mRefCount, texture->mTextureID, texture->mTextureFormat);
    // clang-format on
  }
}  // namespace Theodore
