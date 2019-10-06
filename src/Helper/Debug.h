/**
  @brief console debug utility
  @author bodguy
  @date 17.07.17
  @todo debug log to file support
  @bug
*/

#ifndef Debug_h
#define Debug_h

#include <iostream>
#include <string>

namespace Theodore {
  class Matrix4x4;
  class Vector4d;
  class Vector3d;
  class Vector2d;
  class Color;
  class Quaternion;
  class Ray;
  class Texture;
  class Debug {
  public:
    static void Error(const char* format, ...);
    static void Warn(const char* format, ...);
    static void Trace(const char* format, ...);
    static void Log(const char* format, ...);
    static void Log(const std::string& str);
    static void Log(bool b);
    static void Log(char c);
    static void Log(short s);
    static void Log(unsigned short s);
    static void Log(int i);
    static void Log(unsigned int ui);
    static void Log(long l);
    static void Log(unsigned long ul);
    static void Log(float f);
    static void Log(double d);
    static void Log(const Matrix4x4& matrix, unsigned int precision = 6);
    static void Log(const Vector4d& vector4);
    static void Log(const Vector3d& vector3);
    static void Log(const Vector2d& vector2);
    static void Log(const Color& color);
    static void Log(const Quaternion& quat);
    static void Log(const Ray& ray);
    static void Log(const Texture* object);

  private:
    static const size_t maxLength;
  };
}  // namespace Theodore

#endif /* Debug_h */
