#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
    Texture2D::Texture2D() {
        mType = Enumeration::TextureType;
        mDimension = Enumeration::Tex2D;
    }
    
    Texture2D::~Texture2D() {
        stbi_image_free(mNativeTexturePtr);
        glDeleteTextures(1, &mTextureID);
    }
    
    bool Texture2D::LoadImage(const std::string& filename, Enumeration::TextureFormat format, const Color& colorKey) {
        int w, h, bpp;
        unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, format);
        
        if (data) {
            mColorKey = colorKey;
            mNativeTexturePtr = data;
            mWidth = w;
            mHeight = h;
			SetAssetName(filename);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
            
            // Go through pixels
            // Size of an image = rows * cols * bpp
            // https://www.tutorialspoint.com/dip/concept_of_bits_per_pixel.htm
            GLuint size = mWidth * mHeight * bpp;
            for (GLuint i = 0; i < size; ++i) {
                //Get pixel colors
                GLubyte* colors = (GLubyte*)&data[i];
                
                //Color matches
                if (colors[0] == Color::ConvertToByte(mColorKey.r) &&
                    colors[1] == Color::ConvertToByte(mColorKey.g) &&
                    colors[2] == Color::ConvertToByte(mColorKey.b) && 
					(0 == mColorKey.a || colors[3] == Color::ConvertToByte(mColorKey.a)) ) {
                    //Make transparent
                    colors[0] = 000;
                    colors[1] = 000;
                    colors[2] = 000;
                    colors[3] = 000;
                }
            }
            
            glGenTextures(1, &mTextureID);
            glBindTexture(mDimension, mTextureID);
            switch (format) {
                case Enumeration::RGB24:
                    glTexImage2D(mDimension, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::RGBA32:
                    glTexImage2D(mDimension, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::Red8:
                case Enumeration::Blue8:
                case Enumeration::Green8:
                    break;
            }
            
            // set parameters
            SetMipmapBias(mMipMapBias);
            SetAnisoLevel(mAnisoLevel);
            SetFilter(mFilterMode);
            SetWrapMode(mWrapMode);
            
            glBindTexture(mDimension, NULL);
            
            return true;
        }
        
        return false;
    }
    
    bool Texture2D::LoadImage(const std::string& filename, Enumeration::TextureFormat format) {
        int w, h, bpp;
        unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, format);
        
        if (data) {
            mNativeTexturePtr = data;
            mWidth = w;
            mHeight = h;
			SetAssetName(filename);
            
            glGenTextures(1, &mTextureID);
            glBindTexture(mDimension, mTextureID);
            switch (format) {
                case Enumeration::RGB24:
                    glTexImage2D(mDimension, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::RGBA32:
                    glEnable(GL_ALPHA_TEST);
                    glAlphaFunc(GL_GREATER, 0);
                    glTexImage2D(mDimension, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::Red8:
                case Enumeration::Blue8:
                case Enumeration::Green8:
                    break;
            }
            
            // set parameters
            SetMipmapBias(mMipMapBias);
            SetAnisoLevel(mAnisoLevel);
            SetFilter(mFilterMode);
            SetWrapMode(mWrapMode);
            
            glBindTexture(mDimension, NULL);
            
            return true;
        }
        
        return false;
    }
    
    bool Texture2D::LoadRawTextureData(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& native, const Color& colorKey) {
        int w, h, bpp;
        unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, format);
        
        if (data) {
            mColorKey = colorKey;
            mNativeTexturePtr = data;
            mWidth = w;
            mHeight = h;
			SetAssetName(filename);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
            
            // Go through pixels
            // Size of an image = rows * cols * bpp
            // https://www.tutorialspoint.com/dip/concept_of_bits_per_pixel.htm
            GLuint size = mWidth * mHeight * bpp;
            for (GLuint i = 0; i < size; ++i) {
                //Get pixel colors
                GLubyte* colors = (GLubyte*)&data[i];
                
                //Color matches
                if (colors[0] == Color::ConvertToByte(mColorKey.r) &&
                    colors[1] == Color::ConvertToByte(mColorKey.g) &&
                    colors[2] == Color::ConvertToByte(mColorKey.b) &&
                    (0 == mColorKey.a || colors[3] == Color::ConvertToByte(mColorKey.a))) {
                    //Make transparent
                    colors[0] = 000;
                    colors[1] = 000;
                    colors[2] = 000;
                    colors[3] = 000;
                }
            }
            
            glGenTextures(1, &mTextureID);
            glBindTexture(mDimension, mTextureID);
            switch (format) {
                case Enumeration::RGB24:
                    glTexImage2D(mDimension, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::RGBA32:
                    glTexImage2D(mDimension, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::Red8:
                case Enumeration::Blue8:
                case Enumeration::Green8:
                    break;
            }
            
            // set parameters
            SetMipmapBias(mMipMapBias);
            SetAnisoLevel(mAnisoLevel);
            SetFilter(mFilterMode);
            SetWrapMode(mWrapMode);
            
            glBindTexture(mDimension, NULL);
            
            return true;
        }
        
        return false;
    }
    
    bool Texture2D::LoadRawTextureData(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& native) {
        int w, h, bpp;
        unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, format);
        
        if (data) {
            mNativeTexturePtr = data;
            mWidth = (unsigned int)w;
            mHeight = (unsigned int)h;
			SetAssetName(filename);
            
            glGenTextures(1, &mTextureID);
            glBindTexture(mDimension, mTextureID);
            switch (format) {
                case Enumeration::RGB24:
                    glTexImage2D(mDimension, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::RGBA32:
                    glEnable(GL_ALPHA_TEST);
                    glAlphaFunc(GL_GREATER, 0);
                    glTexImage2D(mDimension, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                case Enumeration::Red8:
                case Enumeration::Blue8:
                case Enumeration::Green8:
                    break;
            }
            
            // set parameters
            SetMipmapBias(mMipMapBias);
            SetAnisoLevel(mAnisoLevel);
            SetFilter(mFilterMode);
            SetWrapMode(mWrapMode);
            
            glBindTexture(mDimension, NULL);
            
            return true;
        }
        
        return false;
    }	
    
    bool Texture2D::LoadCustomTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned char* data) {
        glGenTextures(1, &mTextureID);
        glBindTexture(mDimension, mTextureID);
        mWidth = width;
        mHeight = height;
        
        switch (format) {
            case Enumeration::RGBA32:
                glTexImage2D(mDimension, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
            case Enumeration::RGB24:
                glTexImage2D(mDimension, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                break;
            case Enumeration::Red8:
                glTexImage2D(mDimension, 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                break;
            case Enumeration::Green8:
                glTexImage2D(mDimension, 0, GL_GREEN, mWidth, mHeight, 0, GL_GREEN, GL_UNSIGNED_BYTE, data);
                break;
            case Enumeration::Blue8:
                glTexImage2D(mDimension, 0, GL_BLUE, mWidth, mHeight, 0, GL_BLUE, GL_UNSIGNED_BYTE, data);
                break;
        }
        
        SetFilter(mFilterMode);
        SetWrapMode(mWrapMode);
        glBindTexture(mDimension, NULL);
        
        return true;
    }
}
