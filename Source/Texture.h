/**
	@brief texture interface
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef Texture_h
#define Texture_h

#include "os_types.h"
#include GLEW_INCLUDE_DIR
#include <vector>
#include "Color.h"
#include "Asset.h"
#include "Enumeration.h"

namespace Theodore {
    class Texture : public Asset {
        friend class Debug;
    public:
        Texture();
        virtual ~Texture() = 0;
        
        unsigned char* GetNativeTexturePtr();
        Color GetPixel(unsigned int x, unsigned int y);
        void SetPixel(unsigned int x, unsigned int y, const Color& color);
        
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        unsigned int GetTextureID() const;
        std::string GetMappingType() const;

		Color GetColorKey() const;

        void SetMipmapBias(float bias);
        void SetAnisoLevel(float level);
        void SetFilter(FilterMode mode);
        void SetWrapMode(WrapMode mode);
        void SetMappingType(const std::string& type);
        
        TextureDimension GetDimension() const;
        
    protected:
        unsigned char* mNativeTexturePtr;
        unsigned int mTextureID;
        int mWidth, mHeight;
        float mAnisoLevel;
        float mMipMapBias;
        std::string mMappingType;
        
        FilterMode mFilterMode;
        WrapMode mWrapMode;
        TextureDimension mDimension;
        Color mColorKey;
    };
}

#endif /* Texture_h */
