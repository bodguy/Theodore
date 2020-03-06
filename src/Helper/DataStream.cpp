// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "DataStream.h"

#include <assert.h>

#include <vector>

namespace Theodore {
  DataStream::DataStream(void* b, size_t n) : byteOrder(ByteOrder::BigEndian), precision(FloatingPointPrecision::DoublePrecision), occupied(0) {
		start = (uint8_t*)b;
		data = start;

    if (n == 0)
			size = SIZE_MAX;
    else
			size = n;

    /*
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << QString("the answer is");   // serialize a string
    out << (qint32)42;        // serialize an integer

    QFile file("file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    QString str;
    qint32 a;
    in >> str >> a;           // extract "the answer is" and 42
    */

    /*
    #include <assert.h>

    #include <algorithm>
    #include <future>
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>

    #include "DataStream.h"


    // https://github.com/mkeeter/fstl/blob/master/src/loader.cpp

    struct Verts {
    Verts(float x, float y, float z) :x(x), y(y), z(z) {}
    float x, y, z;

    bool operator<(const Verts& rhs) const {
    return x < rhs.x;
    }
    };

    void parallel_sort(Verts* begin, Verts* end, int threads) {
    if (threads < 2 || end - begin < 2) {
    std::sort(begin, end);
    }
    else {
    Verts* const mid = begin + (end - begin) / 2;
    if (threads == 2) {
    std::future<void> future = std::async(parallel_sort, begin, mid, threads / 2);
    std::sort(mid, end);
    future.wait();
    }
    else {
    std::future<void> a = std::async(std::launch::async, parallel_sort, begin, mid, threads / 2);
    std::future<void> b = std::async(std::launch::async, parallel_sort, mid, end, threads / 2);
    a.wait();
    b.wait();
    }
    std::inplace_merge(begin, mid, end);
    }
    }

    int main() {
    unsigned int threads = std::thread::hardware_concurrency();
    if (threads == 0) {
    threads = 8;
    }

    std::vector<Verts> vec;
    vec.push_back(Verts(0, 0, 0));
    vec.push_back(Verts(1, 1, 1));
    vec.push_back(Verts(6, 6, 6));
    vec.push_back(Verts(4, 4, 4));
    vec.push_back(Verts(2, 2, 2));
    vec.push_back(Verts(7, 7, 7));
    vec.push_back(Verts(3, 3, 3));
    vec.push_back(Verts(9, 9, 9));
    vec.push_back(Verts(5, 5, 5));
    vec.push_back(Verts(8, 8, 8));

    parallel_sort(&(vec.front()), &(vec.front()) + vec.size(), threads);
    for (auto elem : vec) {
    std::cout << elem.x << ", " << elem.y << ", " << elem.z << std::endl;
    }

    getchar();
    return 0;
    }
    */
  }

  DataStream::~DataStream() {}

  size_t DataStream::ReadRawData(void* dst, size_t n) {
    assert(Remaining() >= n);
    uint8_t* d = (uint8_t*)dst;

    if (byteOrder == ByteOrder::LittleEndian) {
      for (size_t i = 0; i < n; i++) {
        *d++ = *data++;
      }
    }

    if (byteOrder == ByteOrder::BigEndian) {
      d += n;
      for (size_t i = 0; i < n; i++) {
        *--d = *data++;
      }
    }

    return n;
  }

  DataStream& DataStream::operator>>(int8_t& i) {
    assert(Remaining() >= 1);
    i = *data++;

    return *this;
  }

  DataStream& DataStream::operator>>(int16_t& i) {
    assert(Remaining() >= 2);
    uint16_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) {
      d = swap_uint16(d);
    }
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(int32_t& i) {
    assert(Remaining() >= 4);
    uint32_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) {
      d = swap_uint32(d);
    }
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(int64_t& i) {
    assert(Remaining() >= 8);
    uint64_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) d = swap_uint64(d);
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(uint8_t& i) {
    assert(Remaining() >= 1);
    i = *data++;

    return *this;
  }

  DataStream& DataStream::operator>>(uint16_t& i) {
    assert(Remaining() >= 2);
    uint16_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) {
      d = swap_uint16(d);
    }
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(uint32_t& i) {
    assert(Remaining() >= 4);
    uint32_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) {
      d = swap_uint32(d);
    }
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(uint64_t& i) {
    assert(Remaining() >= 8);
    uint64_t d;
    d = *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    d <<= 8;
    d |= *data++;
    if (byteOrder == ByteOrder::LittleEndian) d = swap_uint64(d);
    i = d;

    return *this;
  }

  DataStream& DataStream::operator>>(float& i) {
    assert(Remaining() >= 4);
    union {
      uint32_t u;
      float f;
    } d;
    this->operator>>(d.u);
    i = d.f;

    return *this;
  }

  DataStream& DataStream::operator>>(double& i) {
    assert(Remaining() >= 8);
    union {
      uint64_t u;
      double f;
    } d;
    this->operator>>(d.u);
    i = d.f;

    return *this;
  }

  DataStream& DataStream::operator>>(std::string& i) {
    uint8_t buff = ' ';
    std::vector<uint8_t> ret;

    while (buff != '\0') {
      this->operator>>(buff);
      ret.push_back(buff);
    }
    i.assign(ret.begin(), ret.end());

    return *this;
  }

  size_t DataStream::WriteRawData(const void* src, size_t n) {
    assert(Remaining() >= n);
    uint8_t* d = (uint8_t*)src;

    if (byteOrder == ByteOrder::LittleEndian) {
      for (size_t i = 0; i < n; i++) {
        *data++ = *d++;
      }
    }

    if (byteOrder == ByteOrder::BigEndian) {
      d += n;
      for (size_t i = 0; i < n; i++) {
        *data++ = *--d;
      }
    }

    occupied += n;

    return n;
  }

  DataStream& DataStream::operator<<(int8_t i) {
    assert(Remaining() >= 1);
    *data++ = (uint8_t)i;
    occupied += sizeof(uint8_t);

    return *this;
  }

  DataStream& DataStream::operator<<(int16_t i) {
    assert(Remaining() >= 2);
    uint16_t d = (uint16_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint16(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(int16_t);

    return *this;
  }

  DataStream& DataStream::operator<<(int32_t i) {
    assert(Remaining() >= 4);
    uint32_t d = (uint32_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint32(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(int32_t);

    return *this;
  }

  DataStream& DataStream::operator<<(int64_t i) {
    assert(Remaining() >= 8);
    uint64_t d = (uint64_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint64(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(int64_t);

    return *this;
  }

  DataStream& DataStream::operator<<(uint8_t i) {
    assert(Remaining() >= 1);
    *data++ = (uint8_t)i;
    occupied += sizeof(uint8_t);

    return *this;
  }

  DataStream& DataStream::operator<<(uint16_t i) {
    assert(Remaining() >= 2);
    uint16_t d = (uint16_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint16(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(uint16_t);

    return *this;
  }

  DataStream& DataStream::operator<<(uint32_t i) {
    assert(Remaining() >= 4);
    uint32_t d = (uint32_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint32(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(uint32_t);

    return *this;
  }

  DataStream& DataStream::operator<<(uint64_t i) {
    assert(Remaining() >= 8);
    uint64_t d = (uint64_t)i;
    if (byteOrder == ByteOrder::BigEndian) {
      d = swap_uint64(d);
    }
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    d >>= 8;
    *data++ = d;
    occupied += sizeof(uint64_t);

    return *this;
  }

  DataStream& DataStream::operator<<(float i) {
    assert(Remaining() >= 4);
    union {
      uint32_t u;
      float f;
    } d;
    d.f = i;
    this->operator<<(d.u);
    occupied += sizeof(uint32_t);

    return *this;
  }

  DataStream& DataStream::operator<<(double i) {
    assert(Remaining() >= 8);
    union {
      uint64_t u;
      double f;
    } d;
    d.f = i;
    this->operator<<(d.u);
    occupied += sizeof(uint64_t);

    return *this;
  }

  DataStream& DataStream::operator<<(std::string i) {
    size_t size = i.size();
    assert(Remaining() >= size + 1);

    if (size > 0) {
      for (unsigned int j = 0; j < size; j++) {
        this->operator<<((int8_t)i[j]);
      }
      this->operator<<((int8_t)'\0');
    }

    return *this;
  }

  size_t DataStream::GetSize() const { return size; }

  size_t DataStream::GetOccupied() const { return occupied; }

  void DataStream::Reset() { data = start; }

  void DataStream::Seek(size_t pos) {
    assert(pos < size);
		data = start + pos;
  }

  size_t DataStream::Remaining() {
    size_t n = size - ((char*)data - (char*)start);
    return n;
  }

  bool DataStream::AtEnd() { return data == start + size; }

  /*
  void DataStream::WriteFile() {
    if(mDevice) {
      Reset();
      mDevice->WriteBinary(mData, sizeof(uint8_t), mOccupied);
    }
  }
  */

  ByteOrder DataStream::GetByteOrder() const { return byteOrder; }

  FloatingPointPrecision DataStream::GetFloatingPointPrecision() const { return precision; }

  void DataStream::SetByteOrder(ByteOrder bo) { byteOrder = bo; }

  void DataStream::SetFloatingPointPrecision(FloatingPointPrecision precision) { precision = precision; }

  bool DataStream::TestBigEndian() {
    union {
      uint32_t i;
      char c[4];
    } e = {0x01000000};

    return (bool)e.c[0];
  }

  // Reference source https://github.com/Jeroen6/DataStream
  uint16_t DataStream::swap_uint16(uint16_t val) { return (val << 8) | (val >> 8); }

  uint32_t DataStream::swap_uint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
  }

  uint64_t DataStream::swap_uint64(uint64_t val) {
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
  }
};  // namespace Theodore