// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef GlyphInfo_h
#define GlyphInfo_h

#include <map>
#include "Helper/Bitmap.h"

namespace Theodore {
	class GlyphInfo {
	public:
		int bearingX;
		int bearingY;
		int advance;
		int ascender;
		int descender;
		int lineGap;
		unsigned int textureId;
		Bitmap<unsigned char> bitmap;
		std::map<uint32_t, float> kerning;
	};
} // namespace Theodore

#endif /* GlyphInfo_h */
