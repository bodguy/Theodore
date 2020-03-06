// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef InputStream_h
#define InputStream_h

#include <vector>

namespace Theodore {
	class Vector2d;
	class Vector3d;
	class Vector4d;
	class Color;
	class InputStream {
	public:
		void Float(float v);
		void Int8(char v);
		void Int16(short v);
		void Int32(int v);
		void Uint8(unsigned char v);
		void Uint16(unsigned short v);
		void Uint32(unsigned int v);

		void Vec2(const Vector2d& v);
		void Vec3(const Vector3d& v);
		void Vec4(const Vector4d& v);
		void Color4(const Color& v);

		void* Pointer();
		unsigned long Size();

	private:
		std::vector<unsigned char> buffer;
		void Bytes(unsigned char* bytes, unsigned int count);
	};
}

#endif /* InputStream_h */
