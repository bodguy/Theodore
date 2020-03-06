// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "InputStream.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "Math/Vector4d.h"
#include "Math/Color.h"

namespace Theodore {
	void InputStream::Float(float v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Int8(char v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Int16(short v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Int32(int v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Uint8(unsigned char v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Uint16(unsigned short v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Uint32(unsigned int v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Vec2(const Vector2d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Vec3(const Vector3d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Vec4(const Vector4d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void InputStream::Color4(const Color& v) { Bytes((unsigned char*)&v, sizeof(v)); }

	void* InputStream::Pointer() { return &buffer[0]; }

	unsigned long InputStream::Size() { return buffer.size(); }

	void InputStream::Bytes(unsigned char* bytes, unsigned int count) {
		for (unsigned int i = 0; i < count; i++) buffer.push_back(bytes[i]);
	}
}