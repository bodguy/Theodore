#include "Debug.h"
#include "../Asset/Texture2D.h"
#include "../Geometry/Ray.h"
#include "../Graphics/Enumeration.h"
#include "../Math/Color.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Quaternion.h"
#include "../Math/Vector2d.h"
#include "../Math/Vector3d.h"
#include "../Math/Vector4d.h"
#include "../Platform/Time.h"
#include <stdarg.h> // for va_list
#include <stdio.h>  // for vprintf

namespace Theodore {
  int Debug::__tabCount = 0;
  std::string Debug::__tabString = "";
  bool Debug::__logSwitch = false;
  const size_t Debug::maxLength = 256;

  void Debug::Trace(const char* format, ...) {
    char fmt[Debug::maxLength];
    va_list args;
    va_start(args, format);
    snprintf(fmt, Debug::maxLength, "%s [STACK TRACE] %s\n", Time::GetDateTime().c_str(), format);
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

  void Debug::Log(int i) { Debug::Log("%d", i); }

  void Debug::Log(float f) { Debug::Log("%f", f); }

  void Debug::Log(double d) { Debug::Log("%lf", d); }

  void Debug::Log(const Matrix4x4& matrix, unsigned int precision) {
    static const char* presition_table[7] = {"%.0f ", "%.1f ", "%.2f ", "%.3f ", "%.4f ", "%.5f ", "%.6f "};
    Debug::Log("Matrix4x4 = ");

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        printf(presition_table[precision], matrix.m44[i][j]);
      }
      printf("\n");
    }
  }

  void Debug::Log(const Vector4d& vector4) { Debug::Log("Vector4d = x: %f, y: %f, z: %f, w: %f", vector4.x, vector4.y, vector4.z, vector4.w); }

  void Debug::Log(const Vector3d& vector3) { Debug::Log("Vector3d = x: %f, y: %f, z: %f", vector3.x, vector3.y, vector3.z); }

  void Debug::Log(const Vector2d& vector2) { Debug::Log("Vector2d = x: %f, y: %f", vector2.x, vector2.y); }

  void Debug::Log(const Color& color) {
    Debug::Log("Color = r:%f[%d] g:%f[%d] b:%f[%d] a:%f[%d]", color.r, Color::ConvertToByte(color.r), color.g, Color::ConvertToByte(color.g), color.b, Color::ConvertToByte(color.b), color.a,
               Color::ConvertToByte(color.a));
  }

  void Debug::Log(const Quaternion& quat) { Debug::Log("Quaternion = w: %f, x: %f, y: %f, z: %f", quat.w, quat.x, quat.y, quat.z); }

  void Debug::Log(const Ray& ray) { Debug::Log("Origin: (%f, %f, %f), Dir: (%f, %f, %f)", ray.origin.x, ray.origin.y, ray.origin.z, ray.direction.x, ray.direction.y, ray.direction.z); }

  void Debug::Log(const Texture* object) {
    Debug::Log("Texture loaded %s[w=%d, h=%d], ref=%d, id=%d format=%d", object->mName.c_str(), object->mWidth, object->mHeight, object->mRefCount, object->mTextureID, object->mTextureFormat);
  }

  void Debug::LogOnOff(bool b) { Debug::__logSwitch = b; }

  bool Debug::GetLogSwitch() { return Debug::__logSwitch; }
} // namespace Theodore
