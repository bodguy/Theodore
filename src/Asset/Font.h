// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Font_h
#define Font_h

#include <string>
#include <map>
#include <stb/stb_truetype.h>
#include "Asset.h"
#include "Math/Vector2d.h"
#include "Helper/Bitmap.h"

namespace Theodore {
  struct Glyph {
    int bearingX, bearingY;
    int advance;
    Bitmap<unsigned char> bitmap;
    std::map<uint32_t, float> kerning;
    Vector2d uv;
    unsigned int textureID;
  };

  class Font : public Asset {
  public:
    Font();
    virtual ~Font() override;

    bool LoadFont(const std::string& fileName, unsigned int faceIndex, float scale);

  private:
    bool LoadGlyph(const char codepoint, float scale);

  private:
    stbtt_fontinfo* mFace;
    unsigned int mFaceIndex;
    std::map<uint32_t, Glyph> mGlyphTable;
  };
} // namespace Theodore

#endif // Font_h
