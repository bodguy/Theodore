#ifndef Debug_h
#define Debug_h

namespace Quark {
    class Matrix4x4; class Vector4d; class Vector3d; class Vector2d; class Color; class Quaternion; class Ray; class Texture;
    class Debug {
    public:
        static void Log(const char* format, ...);
        static void Log(const Matrix4x4& matrix, unsigned int precision = 6);
        static void Log(const Vector4d& vector4);
        static void Log(const Vector3d& vector3);
        static void Log(const Vector2d& vector2);
        static void Log(const Color& color);
        static void Log(const Quaternion& quat);
        static void Log(const Ray& ray);
        static void Log(const Texture* object);
    };
}

#endif /* Debug_h */
