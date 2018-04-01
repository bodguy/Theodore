#ifndef Texture2D_h
#define Texture2D_h

#include "Enumeration.h"
#include "Texture.h"

namespace Quark {
    class Texture2D : public Texture {
    public:
        Texture2D();
        virtual ~Texture2D();
        
        bool LoadImage(const std::string& filename, Enumeration::TextureFormat format, const Color& colorKey);
        bool LoadImage(const std::string& filename, Enumeration::TextureFormat format);
        bool LoadRawTextureData(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& native, const Color& colorKey);
        bool LoadRawTextureData(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& native);
        bool LoadCustomTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned char* data);
    };
}

#endif /* Texture2D_h */
