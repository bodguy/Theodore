#ifndef Quaternion_h
#define Quaternion_h

#include "Matrix4x4.h"

namespace Theodore {
    class Vector3d;
    class Quaternion {
    public:
        //! default contructor
        Quaternion();
        //! constructor with all elements
        Quaternion(float w, float x, float y, float z);
        Quaternion(float w, const Vector3d& v);
        //! copy constructor
        Quaternion(const Quaternion& other);
        //! assignment operator
        const Quaternion& operator =(Quaternion other);
        
        //! add, add and assignment operator
        Quaternion operator +(const Quaternion& other) const;
        Quaternion& operator +=(const Quaternion& other);
        
        //! subtract, assignment operator
        Quaternion operator -(const Quaternion& other) const;
        Quaternion& operator -=(const Quaternion& other);
        
        //! multiply, assignment operator
        Quaternion operator *(const Quaternion& other) const;
        Quaternion& operator *=(const Quaternion& other);
        //Quaternion operator *(const Vector3d& other) const;
        Quaternion& operator *=(const Vector3d& other);
        Vector3d operator *(const Vector3d& other) const;
        
        //! add by scalar
        Quaternion operator +(const float scalar) const;
        Quaternion& operator +=(const float scalar);
        
        //! subtract by scalar
        Quaternion operator -(const float scalar) const;
        Quaternion& operator -=(const float scalar);
        
        //! multiply by scalar
        Quaternion operator *(const float scalar) const;
        Quaternion& operator *=(const float scalar);
        
        //! divided by scalar
        Quaternion operator /(const float scalar) const;
        Quaternion& operator /=(const float scalar);
        
        //! vector comparison
        bool operator <(const  Quaternion& other) const;
        bool operator >(const  Quaternion& other) const;
        bool operator <=(const Quaternion& other) const;
        bool operator >=(const Quaternion& other) const;
        bool operator ==(const Quaternion& other) const;
        bool operator !=(const Quaternion& other) const;
        
        //! scalar comparison
        bool operator <(const float scalar) const;
        bool operator >(const float scalar) const;
        bool operator <=(const float scalar) const;
        bool operator >=(const float scalar) const;
        
        //! utility functions
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
        //! non-throwing swap function
        void Swap(Quaternion& first, Quaternion& second);
    };
}

#endif /* Quaternion_h */
