#ifndef Matrix4x4_h
#define Matrix4x4_h

namespace Theodore {
	class Vector2d; class Vector3d; class Vector4d; class Quaternion;
    class Matrix4x4 {
    public:
        //! default contructor
        Matrix4x4();
        //! constructor with all elements
        Matrix4x4(float m11, float m12, float m13, float m14,
                  float m21, float m22, float m23, float m24,
                  float m31, float m32, float m33, float m34,
                  float m41, float m42, float m43, float m44);
        //! copy constructor
        Matrix4x4(const Matrix4x4& other);
        //! assignment operator
        Matrix4x4& operator=(Matrix4x4 other);
        
        //! equal, not equal operator
        bool operator==(const Matrix4x4 &other) const;
        bool operator!=(const Matrix4x4 &other) const;
        
        //! add, add and assignment operator
        Matrix4x4 operator+(const Matrix4x4& other) const;
        Matrix4x4& operator+=(const Matrix4x4& other);
        
        //! subtract, assignment operator
        Matrix4x4 operator-(const Matrix4x4& other) const;
        Matrix4x4& operator-=(const Matrix4x4& other);
        
        //! multiply, assignment operator
        Matrix4x4 operator*(const Matrix4x4& other) const;
        Matrix4x4& operator*=(const Matrix4x4& other);
        
        //! multiply by scalar
        Matrix4x4 operator*(const float scalar) const;
        Matrix4x4& operator*=(const float scalar);
        
        //! divided by scalar
        Matrix4x4 operator/(const float scalar) const;
        Matrix4x4& operator/=(const float scalar);
        
        Vector4d operator*(const Vector4d& other);
        
        //! read only accessor
        const float operator()(unsigned int row, unsigned int col) const;
        const float operator[](unsigned int index) const;
        const float At(int index) const;
        const float At(int row, int col) const;
        
        //! Identity matrix
        Matrix4x4& MakeIdentity();
        static Matrix4x4 Identity();
        bool IsIdentity() const;
        
        //! Inverse matrix
        Matrix4x4& MakeInverse();
        static Matrix4x4 Inverse(const Matrix4x4& other);
        
        //! Transpose matrix
        Matrix4x4& MakeTranspose();
        
        //! get native matrix pointer, Note : this is for shader
        const float* Pointer() const;
        
        //! Transform functions
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
		static Matrix4x4 TRS(const Vector3d& pos, const Quaternion& q, const Vector3d& s);
        
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
        };
        
    private:
        //! non-throwing swap function
        void Swap(Matrix4x4& first, Matrix4x4& second);
    };
}

#endif /* Matrix4x4_h */
