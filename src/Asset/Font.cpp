// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#define STB_TRUETYPE_IMPLEMENTATION
#include "Font.h"
#include "Helper/File.h"
#include <cstdlib>

namespace Theodore {
  Font::Font() :mFaceIndex(0) {
    mType = AssetType::FontType;
    mFace = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
  }

  Font::~Font() {
    free(mFace);
  }

  bool Font::LoadFont(const std::string& fileName, unsigned int faceIndex, float scale) {
    File file(fileName, OpenMode::ReadBinary);
    if (!file.IsOpen()) return false;

    size_t size = file.GetSize();
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * size); // no need to free
    if (!buffer) return false;
    file.ReadBuf(buffer, size, 1);
    file.Close();

    if (!stbtt_InitFont(mFace, buffer, faceIndex)) return false;
    mFaceIndex = faceIndex;

    return true;
  }

  bool Font::LoadGlyph(const char codepoint, float scale) {
    int glyphIndex = stbtt_FindGlyphIndex(mFace, codepoint);
    if (glyphIndex == 0) return false;

    Glyph glyph;
    float s = stbtt_ScaleForPixelHeight(mFace, scale);
    unsigned char* buffer = stbtt_GetGlyphBitmapSubpixel(mFace, s, s, 0.f, 0.f, glyphIndex, glyph.bitmap.GetWidthRef(), glyph.bitmap.GetHeightRef(), nullptr, nullptr);

    int width = glyph.bitmap.GetWidth();
    int height = glyph.bitmap.GetHeight();
    Bitmap<unsigned char> rawBitmap(width, height, (unsigned char)0);

    // copy with inverted y
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int inverted_y = (height - 1) - y;
        unsigned char byteBuf = buffer[x + inverted_y * width];
        rawBitmap.SetBit(x, y, byteBuf);
      }
    }
    free(buffer); // don't need anymore
    glyph.bitmap = rawBitmap;

    stbtt_GetCodepointHMetrics(mFace, codepoint, &glyph.advance, &glyph.bearingX);
//    stbtt_GetFont

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // ascii 0~128
    for (int c = 0; c < 128; c++) {

    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // reset the unpacking alignment back to 4 bytes

    return true;
  }
}