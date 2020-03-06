// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "GPUBuffer.h"

namespace Theodore {
	GPUBuffer::GPUBuffer(BufferType type) {
		bufferType = type;
		length = 0;
		glGenBuffers(1, &vertexBufferId);
	}

	GPUBuffer::GPUBuffer(const void* data, size_t length, BufferUsage usage, BufferType type) {
		glGenBuffers(1, &vertexBufferId);
		bufferType = type;
		Data(data, length, usage);
	}

	GPUBuffer::~GPUBuffer() { glDeleteBuffers(1, &vertexBufferId); }

	GPUBuffer::GPUBuffer(const GPUBuffer& other) {
		if (length) {
			glBindBuffer(GL_COPY_READ_BUFFER, other.vertexBufferId);
			glBindBuffer(GL_COPY_WRITE_BUFFER, vertexBufferId);
			glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, length);
		}

		vertexBufferId = other.vertexBufferId;
		bufferType = other.bufferType;
		length = other.length;
	}

	const GPUBuffer& GPUBuffer::operator=(GPUBuffer other) {
		if (length) {
			glBindBuffer(GL_COPY_READ_BUFFER, other.vertexBufferId);
			glBindBuffer(GL_COPY_WRITE_BUFFER, vertexBufferId);
			glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, length);
		}

		vertexBufferId = other.vertexBufferId;
		bufferType = other.bufferType;
		length = other.length;
		return *this;
	}

	unsigned int GPUBuffer::GetBufferID() const { return vertexBufferId; }

	void GPUBuffer::Data(const void* data, size_t length, BufferUsage usage) {
		length = length;
		glBindBuffer(static_cast<GLenum>(bufferType), vertexBufferId);
		glBufferData(static_cast<GLenum>(bufferType), length, data, static_cast<GLenum>(usage));
	}

	void GPUBuffer::SubData(const void* data, size_t offset, size_t length) {
		glBindBuffer(static_cast<GLenum>(bufferType), vertexBufferId);
		glBufferSubData(static_cast<GLenum>(bufferType), offset, length, data);
	}

	void GPUBuffer::GetSubData(void* data, size_t offset, size_t length) {
		glBindBuffer(static_cast<GLenum>(bufferType), vertexBufferId);
		glGetBufferSubData(static_cast<GLenum>(bufferType), offset, length, data);
	}

	void GPUBuffer::BindBase(unsigned int index) { glBindBufferBase(static_cast<GLenum>(bufferType), index, vertexBufferId); }

	void GPUBuffer::BindRange(unsigned int index, unsigned int offset, unsigned int size) { glBindBufferRange(static_cast<GLenum>(bufferType), index, vertexBufferId, offset, size); }

	void* GPUBuffer::Lock(LockMode mode) { return glMapBuffer(static_cast<GLenum>(bufferType), static_cast<GLenum>(mode)); }

	void GPUBuffer::UnLock() { glUnmapBuffer(static_cast<GLenum>(bufferType)); }
}