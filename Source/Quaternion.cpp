#include "Quaternion.h"
#include "Vector3d.h"
#include "Math.h"
#include <algorithm> // until c++11 for std::swap
#include <utility> // since c++11 for std::swap
#include <cmath>

namespace Theodore {
    const Quaternion Quaternion::zero(0.f, 0.f, 0.f, 0.f);
    const Quaternion Quaternion::identity(1.f, 0.f, 0.f, 0.f);
    
    Quaternion::Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {
        
    }
    
    Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {
        
    }
    
    Quaternion::Quaternion(float w, const Vector3d& v) : w(w), x(v.x), y(v.y), z(v.z) {
        
    }
    
    Quaternion::Quaternion(const Quaternion& other) {
        w = other.w;
        x = other.x;
        y = other.y;
        z = other.z;
    }
    
    const Quaternion& Quaternion::operator =(Quaternion other) {
        // copy and swap idiom (effective c++ section 11)
        Swap(*this, other);
        return *this;
    }
    
    Quaternion Quaternion::operator +(const Quaternion& other) const {
        // using op= (more effective c++ section 22)
        return Quaternion(*this) += other;
    }
    
    Quaternion& Quaternion::operator +=(const Quaternion& other) {
        w += other.w;
        x += other.x;
        y += other.y;
        z += other.z;
        
        return *this;
    }
    
    Quaternion Quaternion::operator -(const Quaternion& other) const {
        return Quaternion(*this) -= other;
    }
    
    Quaternion& Quaternion::operator -=(const Quaternion& other) {
        w -= other.w;
        x -= other.x;
        y -= other.y;
        z -= other.z;
        
        return *this;
    }
    
    Quaternion Quaternion::operator *(const Quaternion& other) const {
        return Quaternion(*this) *= other;
    }
    
    Quaternion& Quaternion::operator *=(const Quaternion& other) {
        Quaternion tmp;

        tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
        tmp.x = (other.w * x) + (other.x * w) + (other.z * y) - (other.y * z);
        tmp.y = (other.w * y) + (other.y * w) + (other.x * z) - (other.z * x);
        tmp.z = (other.w * z) + (other.z * w) + (other.y * x) - (other.x * y);
        *this = tmp;
        
        return *this;
    }
    
    //Quaternion Quaternion::operator *(const Vector3d& other) const {
    //	return Quaternion(*this) *= other;
    //}
    
    Quaternion& Quaternion::operator *=(const Vector3d& other) {
        Quaternion tmp;
        
        tmp.w = (other.x * -x) - (other.y * y) - (other.z * z);
        tmp.x = (other.x *  w) - (other.z * y) - (other.y * z);
        tmp.y = (other.y *  w) - (other.x * z) - (other.z * x);
        tmp.z = (other.z *  w) - (other.y * x) - (other.x * y);
        *this = tmp;
        
        return *this;
    }
    
    Vector3d Quaternion::operator *(const Vector3d& other) const {
        float num  =  x * 2.0f;
        float num2 =  y * 2.0f;
        float num3 =  z * 2.0f;
        float num4 =  x * num;
        float num5 =  y * num2;
        float num6 =  z * num3;
        float num7 =  x * num2;
        float num8 =  x * num3;
        float num9 =  y * num3;
        float num10 = w * num;
        float num11 = w * num2;
        float num12 = w * num3;
        
        Vector3d result;
        result.x = (1.f - (num5 + num6)) * other.x + (num7 - num12) * other.y + (num8 + num11) * other.z;
        result.y = (num7 + num12) * other.x + (1.f - (num4 + num6)) * other.y + (num9 - num10) * other.z;
        result.z = (num8 - num11) * other.x + (num9 + num10) * other.y + (1.f - (num4 + num5)) * other.z;
        
        return result;
    }
    
    Quaternion Quaternion::operator +(const float scalar) const {
        return Quaternion(*this) += scalar;
    }
    
    Quaternion& Quaternion::operator +=(const float scalar) {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        
        return *this;
    }
    
    Quaternion Quaternion::operator -(const float scalar) const {
        return Quaternion(*this) -= scalar;
    }
    
    Quaternion& Quaternion::operator -=(const float scalar) {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        
        return *this;
    }
    
    Quaternion Quaternion::operator *(const float scalar) const {
        return Quaternion(*this) *= scalar;
    }
    
    Quaternion& Quaternion::operator *=(const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        
        return *this;
    }
    
    Quaternion Quaternion::operator /(const float scalar) const {
        return Quaternion(*this) /= scalar;
    }
    
    Quaternion& Quaternion::operator /=(const float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        
        return *this;
    }
    
    bool Quaternion::operator <(const  Quaternion& other) const {
        return x < other.x &&
        y < other.y &&
        z < other.z &&
        w < other.w;
    }
    
    bool Quaternion::operator >(const  Quaternion& other) const {
        return x > other.x &&
        y > other.y &&
        z > other.z &&
        w > other.w;
    }
    
    bool Quaternion::operator <=(const Quaternion& other) const {
        return x <= other.x &&
        y <= other.y &&
        z <= other.z &&
        w <= other.w;
    }
    
    bool Quaternion::operator >=(const Quaternion& other) const {
        return x >= other.x &&
        y >= other.y &&
        z >= other.z &&
        w >= other.w;
    }
    
    bool Quaternion::operator ==(const Quaternion& other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }
    
    bool Quaternion::operator !=(const Quaternion& other) const {
        return !(*this == other);
    }
    
    bool Quaternion::operator <(const float scalar) const {
        return x < scalar &&
        y < scalar &&
        z < scalar &&
        w < scalar;
    }
    
    bool Quaternion::operator >(const float scalar) const {
        return x > scalar &&
        y > scalar &&
        z > scalar &&
        w > scalar;
    }
    
    bool Quaternion::operator <=(const float scalar) const {
        return x <= scalar &&
        y <= scalar &&
        z <= scalar &&
        w <= scalar;
    }
    
    bool Quaternion::operator >=(const float scalar) const {
        return x >= scalar &&
        y >= scalar &&
        z >= scalar &&
        w >= scalar;
    }
    
    Quaternion& Quaternion::Conjugate() {
        x = -x;
        y = -y;
        z = -z;
        
        return *this;
    }
    
    Quaternion& Quaternion::Normalize() {
        float n = Length();
        if (n == 1.f || n == 0.f)
            return *this;
        
        n = 1.0f / n;
        return *this *= n;
    }
    
    Quaternion& Quaternion::MakeInverse() {
        float len = Length();
        (*this).Conjugate() /= len;
        
        return *this;
    }
    
    float Quaternion::Length() const {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }
    
    Vector3d Quaternion::ConjugateProduct(const Vector3d& v) const {
        Quaternion q2(0.f,v.x,v.y,v.z), q = *this, qinv = q;
        q.Conjugate();
        
        q = q*q2*qinv;
        return Vector3d(q.x,q.y,q.z);
    }
    
    float Quaternion::DotProduct(const Quaternion& a, const Quaternion& b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    }
    
    Quaternion Quaternion::Inverse(const Quaternion& other) {
        Quaternion tmp(other);
        return tmp.Conjugate() /= tmp.Length();
    }
    
    Vector3d Quaternion::ToEuler(const Quaternion& quat) {
		// NEED TEST
        const float sqw = quat.w*quat.w;
        const float sqx = quat.x*quat.x;
        const float sqy = quat.y*quat.y;
        const float sqz = quat.z*quat.z;
        const float test = 2.0f * (quat.y*quat.w - quat.x*quat.z);
        
        Vector3d eulerAngle;
        if (Math::IsEqual(test, 1.0f)) {
            // heading = rotation about z-axis
            eulerAngle.z = -2.0f * std::atan2(quat.x, quat.w);
            // bank = rotation about x-axis
            eulerAngle.x = 0.f;
            // attitude = rotation about y-axis
            eulerAngle.y = Math::pi / 2.0f;
        } else if (Math::IsEqual(test, -1.0)) {
            // heading = rotation about z-axis
            eulerAngle.z = 2.0f * std::atan2(quat.x, quat.w);
            // bank = rotation about x-axis
            eulerAngle.x = 0.f;
            // attitude = rotation about y-axis
            eulerAngle.y = Math::pi / -2.0f;
        } else {
            // heading = rotation about z-axis
            eulerAngle.z = std::atan2(2.0f * (quat.x*quat.y + quat.z*quat.w), (sqx - sqy - sqz + sqw));
            // bank = rotation about x-axis
            eulerAngle.x = std::atan2(2.0f * (quat.y*quat.z + quat.x*quat.w), (-sqx - sqy + sqz + sqw));
            // attitude = rotation about y-axis
            eulerAngle.y = std::asin(Math::Clamp(test, -1.0, 1.0));
        }
        
        return eulerAngle;
    }
    
    Quaternion Quaternion::FromEuler(const Vector3d& euler) {
        /*
         Qx = [ cos(a/2), (sin(a/2), 0, 0)]
         Qy = [ cos(b/2), (0, sin(b/2), 0)]
         Qz = [ cos(c/2), (0, 0, sin(c/2))]
         
         Qx * Qy * Qz
         */
        Quaternion qx(std::cos(euler.x * 0.5f), std::sin(euler.x * 0.5f), 0.f, 0.f);
        Quaternion qy(std::cos(euler.y * 0.5f), 0.f, std::sin(euler.y * 0.5f), 0.f);
        Quaternion qz(std::cos(euler.z * 0.5f), 0.f, 0.f, std::sin(euler.z * 0.5f));
        
        return qx * qy * qz;
    }
    
    void Quaternion::AxisAngle(Vector3d& axis, float& angle) {
        const float scale = std::sqrt(x*x + y*y + z*z);
        
        if (Math::IsZero(scale) || w > 1.0f || w < -1.0f) {
            angle = 0.0f;
            axis.x = 0.0f;
            axis.y = 1.0f;
            axis.z = 0.0f;
        }
        else {
            const float invscale = 1.0f / scale;
            angle = 2.0f * acosf(w);
            axis.x = z * invscale;
            axis.y = y * invscale;
            axis.z = z * invscale;
        }
    }
    
    Quaternion Quaternion::AngleAxis(const float angle, const Vector3d& axis) {
        const float halfAngle = angle * 0.5f;
        const float sin_half = std::sin(halfAngle);
        Quaternion result;
        
        result.w = std::cos(halfAngle);
        result.x = sin_half * axis.x;
        result.y = sin_half * axis.y;
        result.z = sin_half * axis.z;
        
        return result;
    }
    
    //Quaternion Quaternion::FromToRotation(const Vector3d& fromDirection, const Vector3d& toDirection) {
    //	vector3df v0 = from;
    //	vector3df v1 = to;
    //	v0.normalize();
    //	v1.normalize();
    
    //	const f32 d = v0.dotProduct(v1);
    //	if (d >= 1.0f) // If dot == 1, vectors are the same
    //	{
    //		return makeIdentity();
    //	}
    //	else if (d <= -1.0f) // exactly opposite
    //	{
    //		core::vector3df axis(1.0f, 0.f, 0.f);
    //		axis = axis.crossProduct(v0);
    //		if (axis.getLength() == 0)
    //		{
    //			axis.set(0.f, 1.f, 0.f);
    //			axis = axis.crossProduct(v0);
    //		}
    //		// same as fromAngleAxis(core::PI, axis).normalize();
    //		return set(axis.X, axis.Y, axis.Z, 0).normalize();
    //	}
    
    //	const f32 s = sqrtf((1 + d) * 2); // optimize inv_sqrt
    //	const f32 invs = 1.f / s;
    //	const vector3df c = v0.crossProduct(v1)*invs;
    //	return set(c.X, c.Y, c.Z, s * 0.5f).normalize();
    //}
    
    Matrix4x4 Quaternion::ToRotationMatrix(const Quaternion& other) {
        /*
         1-2y^2-2z^2		2xy-2wz		 2xz+2wy		0
         2xy+2wz			1-2x^2-2z^2	 2yz-2wx		0
         2xz-2wy			2yz+2wx		 1-2x^2-2y^2	0
         0				0			 0				1
         */
        Matrix4x4 result;
        
        float xx = other.x * other.x;
        float yy = other.y * other.y;
        float zz = other.z * other.z;
        float xz = other.x * other.z;
        float xy = other.x * other.y;
        float yz = other.y * other.z;
        float wx = other.w * other.x;
        float wy = other.w * other.y;
        float wz = other.w * other.z;
        
        result.m44[0][0] = 1.0f - 2.0f * (yy + zz);
        result.m44[0][1] = 2.0f * (xy - wz);
        result.m44[0][2] = 2.0f * (xz + wy);
        result.m44[0][3] = 0.0f;
        
        result.m44[1][0] = 2.0f * (xy + wz);
        result.m44[1][1] = 1.0f - 2.0f * (xx + zz);
        result.m44[1][2] = 2.0f * (yz - wx);
        result.m44[1][3] = 0.0f;
        
        result.m44[2][0] = 2.0f * (xz - wy);
        result.m44[2][1] = 2.0f * (yz + wx);
        result.m44[2][2] = 1.0f - 2.0f * (xx + yy);
        result.m44[2][3] = 0.0f;
        
        result.m44[3][0] = 0.0f;
        result.m44[3][1] = 0.0f;
        result.m44[3][2] = 0.0f;
        result.m44[3][3] = 1.0f;
        
        return result;
    }
    
    Quaternion Quaternion::FromRotationMatrix(const Matrix4x4& other) {
        float w = std::sqrt(other.m44[0][0] + other.m44[1][1] + other.m44[2][2] + 1.0f) / 2.f;
        float x = (other.m44[2][1] - other.m44[1][2]) / (4 * w);
        float y = (other.m44[0][2] - other.m44[2][0]) / (4 * w);
        float z = (other.m44[1][0] - other.m44[0][1]) / (4 * w);
        
        return Quaternion(w, x, y, z);
    }
    
    Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t) {
        const float s = 1.0f - t;
        return ((a*s) + (b*t));
    }
    
    Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion c = b;
        float angle = DotProduct(a, b);
        
        // If cosTheta < 0, the interpolation will take the long way around the sphere. 
        // To fix this, one quat must be negated.
        if (angle < 0.0f) {
            c = b * -1.0f;
            angle *= -1.0f;
        }
        
        if (angle <= (1 - 0.05f)) {
            const float theta = std::acos(angle);
            return (a * std::sin((1.0f - t) * theta) + c * std::sin(t * theta)) / std::sin(theta);
        }
        
        // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
        return Lerp(a, b, t);
    }
    
    void Quaternion::Swap(Quaternion& first, Quaternion& second) {
        using std::swap;
        
        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.z, second.z);
        swap(first.w, second.w);
    }
}
