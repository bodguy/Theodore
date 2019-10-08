// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Rect_h
#define Rect_h

namespace Theodore {
  class Rect {
  public:
    //! default contructor
    Rect();
    //! constructor with all elements
    Rect(const float _left, const float _top, const float _right, const float _bottom);
    //! copy constructor
    Rect(const Rect& other);
    //! assignment operator
    Rect& operator=(Rect other);

    //! equal, not equal operator
    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    //! add, add and assignment operator
    Rect operator+(const Rect& other) const;
    Rect& operator+=(const Rect& other);

    //! subtract, assignment operator
    Rect operator-(const Rect& other) const;
    Rect& operator-=(const Rect& other);

    //! multiply, assignment operator
    Rect operator*(const Rect& other) const;
    Rect& operator*=(const Rect& other);

    //! Divided, assignment operator
    Rect operator/(const Rect& other) const;
    Rect& operator/=(const Rect& other);

    Rect& SetRect(const float _left, const float _top, const float _right, const float _bottom);

  public:
    // anonymous union
    union {
      struct {
        float left, top;
        float right, bottom;
      };
    };

    static const Rect zero, one;

  private:
    //! non-throwing swap function
    void Swap(Rect& first, Rect& second) noexcept;
  };
}  // namespace Theodore

#endif /* Rect_h */
