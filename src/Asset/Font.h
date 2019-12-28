// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Font_h
#define Font_h

#include <stb/stb_truetype.h>

#include <map>
#include <string>

#include "Asset.h"
#include "Helper/Bitmap.h"
#include "Math/Vector2d.h"

namespace Theodore {
  struct GlyphInfo {
    int bearingX, bearingY;
    int advance;
    int ascender, descender, line_gap;
    unsigned int texture_id;
    Bitmap<unsigned char> bitmap;
    std::map<uint32_t, float> kerning;
  };

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
    stbtt_fontinfo* mFace;
    unsigned char* mFaceBuffer;
    int mPixelHeight;
    int mAscender, mDescender;
    int mLineGap;
    std::map<uint32_t, GlyphInfo*> mGlyphMap;
  };
}  // namespace Theodore

#endif  // Font_h
