// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Rect_h
#define Rect_h

#include <utility>  // since c++11 for std::swap

namespace Theodore {
  template <typename T>
  class Rect {
  public:
    //! default contructor
    Rect();
    //! constructor with all elements
    Rect(const T _left, const T _top, const T _right, const T _bottom);
    //! copy constructor
    Rect(const Rect<T>& other);
    //! assignment operator
    Rect<T>& operator=(Rect<T> other);

    //! equal, not equal operator
    bool operator==(const Rect<T>& other) const;
    bool operator!=(const Rect<T>& other) const;

    //! add, add and assignment operator
    Rect<T> operator+(const Rect<T>& other) const;
    Rect<T>& operator+=(const Rect<T>& other);

    //! subtract, assignment operator
    Rect<T> operator-(const Rect<T>& other) const;
    Rect<T>& operator-=(const Rect<T>& other);

    //! multiply, assignment operator
    Rect<T> operator*(const Rect<T>& other) const;
    Rect<T>& operator*=(const Rect<T>& other);

    //! Divided, assignment operator
    Rect<T> operator/(const Rect<T>& other) const;
    Rect<T>& operator/=(const Rect<T>& other);

    Rect<T>& SetRect(const T _left, const T _top, const T _right, const T _bottom);

  public:
    // anonymous union
    union {
      struct {
        T left, top;
        T right, bottom;
      };
    };

    static const Rect<T> zero, one;

  private:
    //! non-throwing swap function
    void Swap(Rect<T>& first, Rect<T>& second) noexcept;
  };

  template <typename T>
  Rect<T>::Rect() : left((T)0), right((T)0), top((T)0), bottom((T)0) {}

  template <typename T>
  Rect<T>::Rect(const T _left, const T _top, const T _right, const T _bottom) {
    left = _left;
    top = _top;
    right = _right;
    bottom = _bottom;
  }

  template <typename T>
  Rect<T>::Rect(const Rect& other) {
    left = other.left;
    top = other.top;
    right = other.right;
    bottom = other.bottom;
  }

  template <typename T>
  Rect<T>& Rect<T>::operator=(Rect<T> other) {
    // copy and swap idiom (effective c++ section 11)
    Swap(*this, other);
    return *this;
  }

  template <typename T>
  bool Rect<T>::operator==(const Rect<T>& other) const {
    return left == other.left && right == other.right && top == other.top && bottom == other.bottom;
  }

  template <typename T>
  bool Rect<T>::operator!=(const Rect<T>& other) const { return !(*this == other); }

  template <typename T>
  Rect<T> Rect<T>::operator+(const Rect<T>& other) const {
    // using op= (more effective c++ section 22)
    return Rect<T>(*this) += other;
  }

  template <typename T>
  Rect<T>& Rect<T>::operator+=(const Rect<T>& other) {
    left += other.left;
    top += other.top;
    right += other.right;
    bottom += other.bottom;

    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator-(const Rect<T>& other) const { return Rect<T>(*this) -= other; }

  template <typename T>
  Rect<T>& Rect<T>::operator-=(const Rect<T>& other) {
    left -= other.left;
    top -= other.top;
    right -= other.right;
    bottom -= other.bottom;

    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator*(const Rect<T>& other) const { return Rect<T>(*this) *= other; }

  template <typename T>
  Rect<T>& Rect<T>::operator*=(const Rect<T>& other) {
    left *= other.left;
    top *= other.top;
    right *= other.right;
    bottom *= other.bottom;

    return *this;
  }

  template <typename T>
  Rect<T> Rect<T>::operator/(const Rect<T>& other) const { return Rect<T>(*this) /= other; }

  template <typename T>
  Rect<T>& Rect<T>::operator/=(const Rect<T>& other) {
    left /= other.left;
    top /= other.top;
    right /= other.right;
    bottom /= other.bottom;

    return *this;
  }

  template <typename T>
  Rect<T>& Rect<T>::SetRect(const T _left, const T _top, const T _right, const T _bottom) { return *this = Rect<T>(_left, _top, _right, _bottom); }

  template <typename T>
  void Rect<T>::Swap(Rect<T>& first, Rect<T>& second) noexcept {
    using std::swap;
    swap(first.left, second.left);
    swap(first.top, second.top);
    swap(first.right, second.right);
    swap(first.bottom, second.bottom);
  }

  template <typename T>
  const Rect<T> Rect<T>::zero = Rect<T>();
  template <typename T>
  const Rect<T> Rect<T>::one = Rect<T>((T)1, (T)1, (T)1, (T)1);
}  // namespace Theodore

#endif /* Rect_h */
