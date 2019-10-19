// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Bitmap_h
#define Bitmap_h

#include <vector>

namespace Theodore {
  template<typename T>
  class Bitmap {
  public:
    Bitmap();
    Bitmap(int w, int h, T val);
    explicit Bitmap(const Bitmap<T>& other);
    Bitmap<T>& operator =(const Bitmap<T>& other);

    void Clear(int w, int h, T val);

    T GetBit(const int x, const int y) const;
    void SetBit(const int x, const int y, const T val);
    bool ReplaceSubBit(Bitmap<T> const& other, int x_left, int y_bottom);

    int GetWidth() const;
    int GetHeight() const;
    void SetWidth(int w);
    void SetHeight(int h);
    T* GetNativePointer();

  private:
    int GetIndex(const int x, const int y) const;

  private:
    int mWidth, mHeight;
    std::vector<T> mBuffer;
  };

  template <typename T>
  Bitmap<T>::Bitmap() :mWidth(0), mHeight(0), mBuffer() {

  }

  template <typename T>
  Bitmap<T>::Bitmap(int w, int h, T val) :mWidth(w), mHeight(h), mBuffer() {
    Clear(w, h, val);
  }

  template <typename T>
  Bitmap<T>::Bitmap(const Bitmap<T>& other) {
    ReplaceSubBit(other, 0, 0);
  }

  template <typename T>
  Bitmap<T>& Bitmap<T>::operator =(const Bitmap<T>& other) {
    ReplaceSubBit(other, 0, 0);
    return *this;
  }

  template <typename T>
  void Bitmap<T>::Clear(int w, int h, T val) {
    mBuffer.resize(w * h);
    std::fill(mBuffer.begin(), mBuffer.end(), val);
  }

  template <typename T>
  int Bitmap<T>::GetIndex(const int x, const int y) const {
    if (x < 0 || y < 0 || x > mWidth || y > mHeight) {
      return -1;
    }
    int rows_from_bottom = (mHeight - 1) - y;
    int arr_pos = mWidth * rows_from_bottom;
    arr_pos += x;
    return arr_pos;
  }

  template <typename T>
  T Bitmap<T>::GetBit(const int x, const int y) const {
    int idx = GetIndex(x, y);
    assert(idx >= 0);
    return mBuffer[idx];
  }

  template <typename T>
  void Bitmap<T>::SetBit(const int x, const int y, const T val) {
    int idx = GetIndex(x, y);
    if (idx < 0) {
      return;
    }
    mBuffer[idx] = val;
  }

  template <typename T>
  bool Bitmap<T>::ReplaceSubBit(Bitmap<T> const& other, int x_left, int y_bottom) {
    if ((x_left + other.mWidth) > mWidth || (y_bottom + other.mHeight) > mHeight) {
      return false;
    }

    for (int row = 0; row < other.mHeight; row ++) {
      for (int col = 0; col < other.mWidth; col++) {
        SetBit(col + x_left, row + y_bottom, other.GetBit(col, row));
      }
    }
    return true;
  }

  template <typename T>
  int Bitmap<T>::GetWidth() const {
    return mWidth;
  }

  template <typename T>
  int Bitmap<T>::GetHeight() const {
    return mHeight;
  }

  template <typename T>
  void Bitmap<T>::SetWidth(int w) {
    mWidth = w;
  }

  template <typename T>
  void Bitmap<T>::SetHeight(int h) {
    mHeight = h;
  }

  template <typename T>
  T* Bitmap<T>::GetNativePointer() {
    return mBuffer.data();
  }
}

#endif // Bitmap_h
