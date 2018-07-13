#include "VertexBuffer.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include "Color.h"

namespace Theodore {
    
    ////////////////////////////////////////////////////////////////////////////////////
    // InputStream
    
    void InputStream::Float(float v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Int8(char v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Int16(short v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Int32(int v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Uint8(unsigned char v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Uint16(unsigned short v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Uint32(unsigned int v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Vec2(const Vector2d& v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Vec3(const Vector3d& v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Vec4(const Vector4d& v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void InputStream::Color4(const Color& v) {
        Bytes((unsigned char*)&v, sizeof(v));
    }
    
    void* InputStream::Pointer() {
        return &mData[0];
    }
    
    unsigned long InputStream::Size() {
        return mData.size();
    }
    
    void InputStream::Bytes(unsigned char* bytes, unsigned int count) {
        for (unsigned int i = 0; i < count; i++)
            mData.push_back(bytes[i]);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Buffer
    
    Buffer::Buffer(BufferType type) {
        mType = type;
        mLength = 0;
        glGenBuffers(1, &mVertexBufferID);
    }
    
    Buffer::Buffer(const void* data, size_t length, BufferUsage usage, BufferType type) {
        glGenBuffers(1, &mVertexBufferID);
        mType = type;
        Data(data, length, usage);
    }
    
    Buffer::~Buffer() {
        glDeleteBuffers(1, &mVertexBufferID);
    }
    
    Buffer::Buffer(const Buffer& other) {
        if (mLength) {
            glBindBuffer(GL_COPY_READ_BUFFER, other.mVertexBufferID);
            glBindBuffer(GL_COPY_WRITE_BUFFER, mVertexBufferID);
            glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mLength);
        }
        
        mVertexBufferID = other.mVertexBufferID;
        mType = other.mType;
        mLength = other.mLength;
    }
    
    const Buffer& Buffer::operator=(Buffer other) {
        if (mLength) {
            glBindBuffer(GL_COPY_READ_BUFFER, other.mVertexBufferID);
            glBindBuffer(GL_COPY_WRITE_BUFFER, mVertexBufferID);
            glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, mLength);
        }
        
        mVertexBufferID = other.mVertexBufferID;
        mType = other.mType;
        mLength = other.mLength;
        return *this;
    }
    
    unsigned int Buffer::GetBufferID() const {
        return mVertexBufferID;
    }
    
    void Buffer::Data(const void* data, size_t length, BufferUsage usage) {
        mLength = length;
        glBindBuffer(static_cast<GLenum>(mType), mVertexBufferID);
        glBufferData(static_cast<GLenum>(mType), length, data, static_cast<GLenum>(usage));
    }
    
    void Buffer::SubData(const void* data, size_t offset, size_t length) {
        glBindBuffer(static_cast<GLenum>(mType), mVertexBufferID);
        glBufferSubData(static_cast<GLenum>(mType), offset, length, data);
    }
    
    void Buffer::GetSubData(void* data, size_t offset, size_t length) {
        glBindBuffer(static_cast<GLenum>(mType), mVertexBufferID);
        glGetBufferSubData(static_cast<GLenum>(mType), offset, length, data);
    }
    
    void Buffer::BindBase(unsigned int index) {
        glBindBufferBase(static_cast<GLenum>(mType), index, mVertexBufferID);
    }
    
    void Buffer::BindRange(unsigned int index, unsigned int offset, unsigned int size) {
        glBindBufferRange(static_cast<GLenum>(mType), index, mVertexBufferID, offset, size);
    }
    
    void* Buffer::Lock(LockMode mode) {
        return glMapBuffer(static_cast<GLenum>(mType), static_cast<GLenum>(mode));
    }
    
    void Buffer::UnLock() {
        glUnmapBuffer(static_cast<GLenum>(mType));
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    // VertexArray
    
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &mVertexArrayID);
    }
    
    VertexArray::VertexArray(const VertexArray& other) {
        mVertexArrayID = other.mVertexArrayID;
    }
    
    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &mVertexArrayID);
    }
    
    unsigned int VertexArray::GetArrayID() const {
        return mVertexArrayID;
    }
    
    const VertexArray& VertexArray::operator=(const VertexArray& other) {
        mVertexArrayID = other.mVertexArrayID;
        return *this;
    }
    
    void VertexArray::BindAttribute(const Attribute& attribute, const Buffer& buffer, unsigned int count, unsigned int stride, unsigned int offset) {
        glBindVertexArray(mVertexArrayID);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.GetBufferID());
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, count, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
        glBindVertexArray(NULL);
    }
    
    void VertexArray::BindElements(const Buffer& elements) {
        glBindVertexArray(mVertexArrayID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements.GetBufferID());
        glBindVertexArray(NULL);
    }
    
    void VertexArray::AttributeDivisor(const Attribute& attribute, unsigned int divisor) {
        glBindVertexArray(mVertexArrayID);
        glVertexAttribDivisor(attribute, divisor);
        glBindVertexArray(NULL);
    }
}
