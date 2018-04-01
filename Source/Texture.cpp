#include "Texture.h"

namespace Quark {
    Texture::Texture() : mNativeTexturePtr(nullptr), mTextureID(1), mWidth(0), mHeight(0), mMipMapBias(1000.f),
    mFilterMode(Enumeration::Trilinear), mWrapMode(Enumeration::ClampEdge), mDimension(Enumeration::None), mColorKey(Color::white) {
        mType = Enumeration::TextureType;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mAnisoLevel);
    }
    
    Texture::~Texture() {
        
    }
    
    unsigned char* Texture::GetNativeTexturePtr() {
        return mNativeTexturePtr;
    }
    
    Color Texture::GetPixel(unsigned int x, unsigned int y) {
        GLubyte* colors = (GLubyte*)&mNativeTexturePtr[y * mWidth + x];
        return Color(Color::ConvertToFloat(colors[0]), Color::ConvertToFloat(colors[1]),
                     Color::ConvertToFloat(colors[2]), colors[3] ? Color::ConvertToFloat(colors[3]) : 0.f);
    }
    
    void Texture::SetPixel(unsigned int x, unsigned int y, const Color& color) {
        GLubyte* colors = (GLubyte*)&mNativeTexturePtr[y * mWidth + x];
        colors[0] = Color::ConvertToByte(color.r); // r
        colors[1] = Color::ConvertToByte(color.g); // g
        colors[2] = Color::ConvertToByte(color.b); // b
        colors[3] = Color::ConvertToByte(color.a); // a
    }
    
    unsigned int Texture::GetWidth() const {
        return mWidth;
    }
    
    unsigned int Texture::GetHeight() const {
        return mHeight;
    }
    
    unsigned int Texture::GetTextureID() const {
        return mTextureID;
    }
    
    std::string Texture::GetMappingType() const {
        return mMappingType;
    }
    
    void Texture::SetMipmapBias(float bias) {
        // Set mipmap bias value
        if (mFilterMode == Enumeration::Trilinear) {
            glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MIN_LOD, -mMipMapBias);
            glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MAX_LOD, mMipMapBias);
        }
    }
    
    void Texture::SetAnisoLevel(float level) {
        // Set anisotrophy filtering
        glTexParameterf(mDimension, GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisoLevel);
    }
    
    void Texture::SetFilter(Enumeration::FilterMode mode) {
        // Set filtering mode
        glTexParameteri(mDimension, GL_TEXTURE_MIN_FILTER, mFilterMode);
        glTexParameteri(mDimension, GL_TEXTURE_MAG_FILTER, mFilterMode);
        if (mFilterMode == Enumeration::Trilinear)
            glGenerateMipmap(mDimension);
    }
    
    void Texture::SetWrapMode(Enumeration::WrapMode mode) {
        // Set wrap mode
        glTexParameteri(mDimension, GL_TEXTURE_WRAP_S, mWrapMode);
        glTexParameteri(mDimension, GL_TEXTURE_WRAP_T, mWrapMode);
        if(mDimension == Enumeration::Cube) 
            glTexParameteri(mDimension, GL_TEXTURE_WRAP_R, mWrapMode);
    }
    
    void Texture::SetMappingType(const std::string& type) {
        mMappingType = type;
    }
    
    Enumeration::TextureDimension Texture::GetDimension() const {
        return mDimension;
    }
}
