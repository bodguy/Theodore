#ifndef Vector3d_h
#define Vector3d_h

#include "Vector2d.h"
#include "Vector4d.h"

namespace Quark {
    class Vector3d {
    public:
        //! default contructor
        Vector3d();
        //! constructor with three elements
        Vector3d(const float tx, const float ty, const float tz);
        explicit Vector3d(const Vector2d& other);
        Vector3d(const Vector2d& other, float tz);
        //! constructor with two elements
        Vector3d(const float tx, const float ty);
        //! copy constructor
        Vector3d(const Vector3d& other);
		explicit Vector3d(const Vector4d& other);
        //! assignment operator
        Vector3d& operator = (Vector3d other);
		float operator[] (unsigned int i) const;

        //! add, add and assignment operator
        Vector3d operator +(const Vector3d& other) const;
        Vector3d& operator +=(const Vector3d& other);
        
        //! subtract, assignment operator
        Vector3d operator -(const Vector3d& other) const;
        Vector3d& operator -=(const Vector3d& other);
        
        //! multiply, assignment operator
        Vector3d operator *(const Vector3d& other) const;
        Vector3d& operator *=(const Vector3d& other);
        
        Vector3d operator /(const Vector3d& other) const;
        Vector3d& operator /=(const Vector3d& other);
        
        static Vector3d CrossProduct(const Vector3d& a, const Vector3d& b);
        static float DotProduct(const Vector3d& a, const Vector3d& b);
        
        //! add by scalar
        Vector3d operator +(const float scalar) const;
        Vector3d& operator +=(const float scalar);
        
        //! subtract by scalar
        Vector3d operator -(const float scalar) const;
        Vector3d& operator -=(const float scalar);
        
        //! multiply by scalar
        Vector3d operator *(const float scalar) const;
        Vector3d& operator *=(const float scalar);
        
        //! divided by scalar
        Vector3d operator /(const float scalar) const;
        Vector3d& operator /=(const float scalar);
        
        //! vector comparison
        bool operator <(const Vector3d& other) const;
        bool operator >(const Vector3d& other) const;
        bool operator <=(const Vector3d& other) const;
        bool operator >=(const Vector3d& other) const;
        bool operator ==(const Vector3d& other) const;
        bool operator !=(const Vector3d& other) const;
        
        //! scalar comparison
        bool operator <(const float scalar) const;
        bool operator >(const float scalar) const;
        bool operator <=(const float scalar) const;
        bool operator >=(const float scalar) const;
        bool operator ==(const float scalar) const;
        bool operator !=(const float scalar) const;
        
        //! unary operator
        Vector3d operator -() const;
        Vector3d operator +() const;
        Vector3d& Negate();
        
        //! utility functions
        static Vector3d Inverse(const Vector3d& other);
        Vector3d Perpendicular();
        Vector3d& Normalize();
        float Length() const;
        float Distance(const Vector3d& other) const;
        float DistanceSqrare(const Vector3d& other) const;

		Vector2d xx() { return Vector2d(x, x); }
		Vector2d xy() { return Vector2d(x, y); } 
		Vector2d xz() { return Vector2d(x, z); }
		Vector2d yx() { return Vector2d(y, x); }
		Vector2d yy() { return Vector2d(y, y); }
		Vector2d yz() { return Vector2d(y, z); }
		Vector2d zx() { return Vector2d(z, x); }
		Vector2d zy() { return Vector2d(z, y); }
		Vector2d zz() { return Vector2d(z, z); }

		static Vector2d ToVector2d(const Vector3d& other);
		static Vector4d ToVector4d(const Vector3d& other);

        //! linear interpolation
        static Vector3d Lerp(const Vector3d& a, const Vector3d& b, float t); // Linear Interpolation
        static Vector3d Slerp(const Vector3d& a, const Vector3d& b, float t); // Sphere Linear Interpolation
        static Vector3d Nlerp(const Vector3d& a, const Vector3d& b, float t); // Normalized Linear Interpolation
        static Vector3d Project(const Vector3d& vector, const Vector3d& onNormal);
		static Vector3d ProjectOnPlane(const Vector3d& vector, const Vector3d& planeNormal);

        float x, y, z;
        static const Vector3d up, down, left, right, forward, backward, one, zero;
        
    private:
        void Swap(Vector3d& first, Vector3d& second);
    };
}

#endif /* Vector3d_h */
