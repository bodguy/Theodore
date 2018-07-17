/**
	@brief OpenGL msaa texture implementation
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef MSAATexture2D_h
#define MSAATexture2D_h

#include "Texture.h"
#include "Enumeration.h"

namespace Theodore {
	class MSAATexture2D : public Texture {
	public:
		MSAATexture2D();
		virtual ~MSAATexture2D();

		bool LoadMultiSampleTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned int sample);
	};
}

#endif /* MSAATexture2D_h */