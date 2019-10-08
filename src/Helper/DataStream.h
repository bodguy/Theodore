// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef DataStream_h
#define DataStream_h

#include <cstdint>
#include <string>
#include "File.h"
#include "Graphics/Enumeration.h"

namespace Theodore {
  class File;
  class DataStream {
  public:
    DataStream(void* b, size_t n);
    ~DataStream();

    size_t ReadRawData(void* dst, size_t n);
    DataStream& operator>>(int8_t& i);
    DataStream& operator>>(int16_t& i);
    DataStream& operator>>(int32_t& i);
    DataStream& operator>>(int64_t& i);
    DataStream& operator>>(uint8_t& i);
    DataStream& operator>>(uint16_t& i);
    DataStream& operator>>(uint32_t& i);
    DataStream& operator>>(uint64_t& i);
    DataStream& operator>>(float& i);
    DataStream& operator>>(double& i);
    DataStream& operator>>(std::string& i);

    size_t WriteRawData(const void* src, size_t n);
    DataStream& operator<<(int8_t i);
    DataStream& operator<<(int16_t i);
    DataStream& operator<<(int32_t i);
    DataStream& operator<<(int64_t i);
    DataStream& operator<<(uint8_t i);
    DataStream& operator<<(uint16_t i);
    DataStream& operator<<(uint32_t i);
    DataStream& operator<<(uint64_t i);
    DataStream& operator<<(float i);
    DataStream& operator<<(double i);
    DataStream& operator<<(std::string i);

    size_t GetSize() const;
    size_t GetOccupied() const;

    void Reset();
    void Seek(size_t pos);
    size_t Remaining();
    bool AtEnd();

    ByteOrder GetByteOrder() const;
    FloatingPointPrecision GetFloatingPointPrecision() const;

    void SetByteOrder(ByteOrder bo);
    void SetFloatingPointPrecision(FloatingPointPrecision precision);
    static bool TestBigEndian();

  private:
    uint16_t swap_uint16(uint16_t val);
    uint32_t swap_uint32(uint32_t val);
    uint64_t swap_uint64(uint64_t val);

  private:
    uint8_t* mStart;  // start of data
    uint8_t* mData;   // current data pointers
    size_t mSize;     // size of data
    size_t mOccupied;
    ByteOrder mByteOrder;
    FloatingPointPrecision mPrecision;
  };
};  // namespace Theodore

#endif /* DataStream_h */