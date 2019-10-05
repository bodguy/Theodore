/**
  @brief vector4d math class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Vector4d_h
#define Vector4d_h

namespace Theodore {
  class Vector2d;
  class Vector3d;
  class Vector4d {
    public:
    //! default contructor
    Vector4d();
    //! constructor with three elements
    Vector4d(float tx, float ty, float tz, float tw);
    explicit Vector4d(const Vector2d& other);
    explicit Vector4d(const Vector3d& other);
    Vector4d(const Vector2d& other, float tz, float tw);
    Vector4d(const Vector3d& other, float tw);
    //! copy constructor
    Vector4d(const Vector4d& other);
    //! assignment operator
    Vector4d& operator=(Vector4d other);
    float operator[](unsigned int i) const;

    //! add, add and assignment operator
    Vector4d operator+(const Vector4d& other) const;
    Vector4d& operator+=(const Vector4d& other);

    //! subtract, assignment operator
    Vector4d operator-(const Vector4d& other) const;
    Vector4d& operator-=(const Vector4d& other);

    //! multiply, assignment operator
    Vector4d operator*(const Vector4d& other) const;
    Vector4d& operator*=(const Vector4d& other);

    Vector4d operator/(const Vector4d& other) const;
    Vector4d& operator/=(const Vector4d& other);

    static float DotProduct(const Vector4d& a, const Vector4d& b);

    //! add by scalar
    Vector4d operator+(const float scalar) const;
    Vector4d& operator+=(const float scalar);

    //! subtract by scalar
    Vector4d operator-(const float scalar) const;
    Vector4d& operator-=(const float scalar);

    //! multiply by scalar
    Vector4d operator*(const float scalar) const;
    Vector4d& operator*=(const float scalar);

    //! divided by scalar
    Vector4d operator/(const float scalar) const;
    Vector4d& operator/=(const float scalar);

    //! vector comparison
    bool operator<(const Vector4d& other) const;
    bool operator>(const Vector4d& other) const;
    bool operator<=(const Vector4d& other) const;
    bool operator>=(const Vector4d& other) const;
    bool operator==(const Vector4d& other) const;
    bool operator!=(const Vector4d& other) const;

    //! scalar comparison
    bool operator<(const float scalar) const;
    bool operator>(const float scalar) const;
    bool operator<=(const float scalar) const;
    bool operator>=(const float scalar) const;
    bool operator==(const float scalar) const;
    bool operator!=(const float scalar) const;

    //! utility functions
    Vector4d& Normalize();
    float Length();
    static Vector3d ToVector3d(const Vector4d& other);
    static Vector4d Absolute(const Vector4d& other);

    float x, y, z, w;
    static const Vector4d up, down, left, right, forward, backward, one, zero;

    private:
    void Swap(Vector4d& first, Vector4d& second);
  };
} // namespace Theodore

#endif /* Vector4d_h */
