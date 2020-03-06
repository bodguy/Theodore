// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef THEODORE_GPUBUFFER_H
#define THEODORE_GPUBUFFER_H

#include "Enumeration.h"

namespace Theodore {
	class GPUBuffer {
	public:
		GPUBuffer(BufferType type);
		GPUBuffer(const void* data, size_t length, BufferUsage usage, BufferType type);
		~GPUBuffer();
		GPUBuffer(const GPUBuffer& other);

		const GPUBuffer& operator=(GPUBuffer other);
		unsigned int GetBufferID() const;

		void Data(const void* data, size_t length, BufferUsage usage);
		// this redefines some or all of the data store for the specified buffer object.
		void SubData(const void* data, size_t offset, size_t length);
		void GetSubData(void* data, size_t offset, size_t length);
		void BindBase(unsigned int index);
		// map a portion of the buffer.
		void BindRange(unsigned int index, unsigned int offset, unsigned int size);
		void* Lock(LockMode mode);
		void UnLock();

	private:
		unsigned int vertexBufferId;
		BufferType bufferType;
		size_t length;
	};
}

#endif //THEODORE_GPUBUFFER_H
