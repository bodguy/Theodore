// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#define STB_TRUETYPE_IMPLEMENTATION
#include "Font.h"

#include <cmath>
#include <cstdlib>

#include "Helper/File.h"
#include "Helper/StringUtil.h"

namespace Theodore {
  Font::Font() : Asset(), faceBuffer(nullptr), pixelHeight(0), ascender(0), descender(0), lineGap(0), loadedGlyphLookUpTable() {
		assetType = AssetType::FontType;
		fontFace = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
    loadedGlyphLookUpTable.clear();
  }

  Font::~Font() {
    if (fontFace) {
      free(fontFace);
			fontFace = nullptr;
    }

    if (faceBuffer) {
      free(faceBuffer);
			faceBuffer = nullptr;
    }

    for (auto& glyph : loadedGlyphLookUpTable) {
      delete glyph.second;
      glyph.second = nullptr;
    }
    loadedGlyphLookUpTable.clear();
  }

  bool Font::InitFont(const std::string& filename, unsigned int index, int pixel_height) {
    File file(filename, OpenMode::ReadBinary);
    if (!file.IsOpen()) return false;

    size_t size = file.GetSize();
		faceBuffer = (unsigned char*)malloc(sizeof(unsigned char) * size);
    if (!faceBuffer) return false;
    file.ReadBuf(faceBuffer, size, 1);
    file.Close();

    if (!stbtt_InitFont(fontFace, faceBuffer, stbtt_GetFontOffsetForIndex(faceBuffer, index))) return false;
    stbtt_GetFontVMetrics(fontFace, &ascender, &descender, &lineGap);
		pixelHeight = pixel_height;

    return true;
  }

  bool Font::LoadGlyph(uint32_t codepoint) {
    if (!fontFace) return false;
    int index = stbtt_FindGlyphIndex(fontFace, codepoint);
    if (index == 0) return false;

    GlyphInfo* glyph = new GlyphInfo();
    if (!LoadGlyphBitmap(glyph, index)) return false;
    auto res = loadedGlyphLookUpTable.insert(std::make_pair(codepoint, glyph));
    if (!res.second) return false;

    return true;
  }

  bool Font::LoadGlyph(const char* ch) { return LoadGlyph(StringUtil::UTF8Decode(ch)); }

  GlyphInfo* Font::FindGlyph(uint32_t codepoint) {
    if (loadedGlyphLookUpTable.find(codepoint) != loadedGlyphLookUpTable.end()) {
      return loadedGlyphLookUpTable[codepoint];
    }

    return nullptr;
  }

  bool Font::LoadGlyphBitmap(GlyphInfo* out, int glyphIndex) {
    float scale = stbtt_ScaleForPixelHeight(fontFace, pixelHeight);
    int width, height;
    unsigned char* buf = stbtt_GetGlyphBitmapSubpixel(fontFace, scale, scale, 0.f, 0.f, glyphIndex, &width, &height, nullptr, nullptr);
    if (!buf) return false;
    float ascender, descender, line_gap;
    stbtt_GetScaledFontVMetrics(fontFace->data, 0, pixelHeight, &ascender, &descender, &line_gap);
    out->ascender = (int)std::ceil(ascender);
    out->descender = (int)std::ceil(descender);
    out->lineGap = (int)std::ceil(line_gap);
    out->bitmap.SetWidth(width);
    out->bitmap.SetHeight(height);
    out->bitmap.Clear(width, height, (unsigned char)0);

    // copy from bottom-top to top-bottom (reverse)
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int inverted_y = (height - 1) - y;
        unsigned char buf_byte = buf[x + inverted_y * width];
        out->bitmap.SetBit(x, y, buf_byte);
      }
    }
    free(buf);
    stbtt_GetGlyphBitmapBox(fontFace, glyphIndex, scale, scale, nullptr, &out->bearingY, nullptr, nullptr);
    stbtt_GetGlyphHMetrics(fontFace, glyphIndex, &out->advance, &out->bearingX);
    out->bearingX = (int)std::ceil(out->bearingX * scale);
    out->advance = (int)std::ceil(out->advance * scale);

    // make opengl texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(static_cast<GLenum>(TextureDimension::Tex2D), textureID);
    glTexImage2D(static_cast<GLenum>(TextureDimension::Tex2D), 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, out->bitmap.GetNativePointer());
    // Set texture options
    glTexParameteri(static_cast<GLenum>(TextureDimension::Tex2D), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(static_cast<GLenum>(TextureDimension::Tex2D), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(static_cast<GLenum>(TextureDimension::Tex2D), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(static_cast<GLenum>(TextureDimension::Tex2D), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(static_cast<GLenum>(TextureDimension::Tex2D), static_cast<GLenum>(NULL));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // reset the unpacking alignment back to 4 bytes
    out->textureId = textureID;

    return true;
  }
}  // namespace Theodore