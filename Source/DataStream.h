#ifndef DataStream_h
#define DataStream_h

#include "Enumeration.h"
#include <cstdint>

namespace Theodore {
	class File;
	class DataStream {
	public:
		DataStream(void* b, size_t n);
		DataStream(File& device);
		~DataStream();

		size_t ReadRawData(void *dst, size_t n);
		DataStream& operator >>(int8_t   &i);
		DataStream& operator >>(int16_t  &i);
		DataStream& operator >>(int32_t  &i);
		DataStream& operator >>(int64_t  &i);
		DataStream& operator >>(uint8_t  &i);
		DataStream& operator >>(uint16_t &i);
		DataStream& operator >>(uint32_t &i);
		DataStream& operator >>(uint64_t &i);
		DataStream& operator >>(float &i);
		DataStream& operator >>(double &i);

		size_t WriteRawData(const void *src, size_t n);
		DataStream& operator <<(int8_t   i);
		DataStream& operator <<(int16_t  i);
		DataStream& operator <<(int32_t  i);
		DataStream& operator <<(int64_t  i);
		DataStream& operator <<(uint8_t  i);
		DataStream& operator <<(uint16_t i);
		DataStream& operator <<(uint32_t i);
		DataStream& operator <<(uint64_t i);
		DataStream& operator <<(float i);
		DataStream& operator <<(double i);

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
		uint8_t* mStart; // start of data
		uint8_t* mData; // current data pointers
		size_t mSize; // size of data
		ByteOrder mByteOrder;
		FloatingPointPrecision mPrecision;
	};
};


#endif /* DataStream_h */