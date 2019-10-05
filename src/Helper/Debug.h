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
#define LOG(x)                                  \
  if (Debug::__logSwitch) {                     \
    std::cout << __tabString << x << std::endl; \
  }
#define TAB()                                    \
  if (Debug::__logSwitch) {                      \
    __tabCount++;                                \
    __tabString = std::string(__tabCount, '\t'); \
  }
#define UNTAB()                                  \
  if (Debug::__logSwitch) {                      \
    __tabCount--;                                \
    __tabString = std::string(__tabCount, '\t'); \
  }

#ifdef _DEBUG
#define FUNCNAME() Debug::LogFuncName __logFuncName(__FUNCTION__)
#else
#define FUNCNAME()
#endif

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

    static struct LogFuncName {
      LogFuncName(const char* fname) {
        LOG(fname)
        TAB()
      }

      ~LogFuncName() { UNTAB() }
    } temp;

    static void LogOnOff(bool b);
    static bool GetLogSwitch();

  private:
    static int __tabCount;
    static std::string __tabString;
    static bool __logSwitch;
    static const size_t maxLength;
  };
}  // namespace Theodore

#endif /* Debug_h */
