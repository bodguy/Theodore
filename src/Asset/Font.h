// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Font_h
#define Font_h

#include <stb/stb_truetype.h>
#include <map>
#include <string>
#include "Asset.h"
#include "Math/Vector2d.h"

namespace Theodore {
	class GlyphInfo;
  class Font : public Asset {
    friend class Debug;

  public:
    Font();
    virtual ~Font() override;

    bool InitFont(const std::string& filename, unsigned int index, int pixel_height);
    bool LoadGlyph(uint32_t codepoint);
    bool LoadGlyph(const char* ch);
    GlyphInfo* FindGlyph(uint32_t codepoint);

  private:
    bool LoadGlyphBitmap(GlyphInfo* out, int glyphIndex);

  private:
    stbtt_fontinfo* fontFace;
    unsigned char* faceBuffer;
    int pixelHeight;
    int ascender, descender;
    int lineGap;
    std::map<uint32_t, GlyphInfo*> loadedGlyphLookUpTable;
  };
} // namespace Theodore

#endif  // Font_h
