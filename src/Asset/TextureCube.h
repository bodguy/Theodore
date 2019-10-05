/**
  @brief cubemap texture datastructure
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef TextureCube_h
#define TextureCube_h

#include "../Graphics/Enumeration.h"
#include "Texture.h"
#include <string>

namespace Theodore {
  class CubemapRenderer;
  class TextureCube : public Texture {
    public:
    TextureCube();
    ~TextureCube();

    bool LoadCubemapTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face);
    CubemapFace GetFace() const;

    private:
    CubemapFace mFace;
  };
} // namespace Theodore

#endif /* TextureCube_h */
