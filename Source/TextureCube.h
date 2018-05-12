#ifndef TextureCube_h
#define TextureCube_h

#include "Enumeration.h"
#include "Texture.h"
#include <string>

namespace Quark {
	class TextureCube : public Texture {
	public:
		TextureCube();
		~TextureCube();

		bool LoadCubemapTexture(unsigned int id, const std::string& filename, TextureFormat format, CubemapFace face);
	};
}

#endif /* TextureCube_h */
