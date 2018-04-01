#ifndef MSAATexture2D_h
#define MSAATexture2D_h

#include "Texture.h"
#include "Enumeration.h"

namespace Quark {
	class MSAATexture2D : public Texture {
	public:
		MSAATexture2D();
		virtual ~MSAATexture2D();

		bool LoadMultiSampleTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned int sample);
	};
}

#endif /* MSAATexture2D_h */