#include "Texture.h"

namespace Theodore {
  Texture::Texture()
      : mNativeTexturePtr(nullptr),
        mTextureID(1),
        mWidth(0),
        mHeight(0),
        mMipMapBias(1000.f),
        mFilterMode(FilterMode::Trilinear),
        mWrapMode(WrapMode::ClampEdge),
        mDimension(TextureDimension::None),
        mTextureFormat(TextureFormat::UNKNOWN),
        mColorKey(Color::white),
        mUseColorKey(false) {
    mType = AssetType::TextureType;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mAnisoLevel);
  }

  Texture::~Texture() {}

  unsigned char* Texture::GetNativeTexturePtr() { return mNativeTexturePtr; }

  Color Texture::GetPixel(unsigned int x, unsigned int y) {
    GLubyte* colors = (GLubyte*)&mNativeTexturePtr[y * mWidth + x];
    return Color::RGBToColor(colors[0], colors[1], colors[2], colors[3] ? colors[3] : 0);
  }

  void Texture::SetPixel(unsigned int x, unsigned int y, const Color& color) {
    GLubyte* colors = (GLubyte*)&mNativeTexturePtr[y * mWidth + x];
    colors[0] = Color::ConvertToByte(color.r);  // r
    colors[1] = Color::ConvertToByte(color.g);  // g
    colors[2] = Color::ConvertToByte(color.b);  // b
    colors[3] = Color::ConvertToByte(color.a);  // a
  }

  unsigned int Texture::GetWidth() const { return mWidth; }

  unsigned int Texture::GetHeight() const { return mHeight; }

  unsigned int Texture::GetTextureID() const { return mTextureID; }

  std::string Texture::GetMappingType() const { return mMappingType; }

  Color Texture::GetColorKey() const { return mColorKey; }

  bool Texture::UseColorKey() const { return mUseColorKey; }

  void Texture::SetMipmapBias(float bias) {
    mMipMapBias = bias;
    // Set mipmap bias value
    if (mFilterMode == FilterMode::Trilinear) {
      glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MIN_LOD, -mMipMapBias);
      glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MAX_LOD, mMipMapBias);
    }
  }

  void Texture::SetAnisoLevel(float level) {
    mAnisoLevel = level;
    // Set anisotrophy filtering
    glTexParameterf(static_cast<GLenum>(mDimension), GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisoLevel);
  }

  void Texture::SetFilter(const FilterMode mode) {
    mFilterMode = mode;
    // Set filtering mode
    glTexParameteri(static_cast<GLenum>(mDimension), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mFilterMode));
    glTexParameteri(static_cast<GLenum>(mDimension), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mFilterMode));
    if (mFilterMode == FilterMode::Trilinear) glGenerateMipmap(static_cast<GLenum>(mDimension));
  }

  void Texture::SetWrapMode(const WrapMode mode) {
    mWrapMode = mode;
    // Set wrap mode
    glTexParameteri(static_cast<GLenum>(mDimension), GL_TEXTURE_WRAP_S, static_cast<GLenum>(mWrapMode));
    glTexParameteri(static_cast<GLenum>(mDimension), GL_TEXTURE_WRAP_T, static_cast<GLenum>(mWrapMode));
    if (mDimension == TextureDimension::CubeMap) glTexParameteri(static_cast<GLenum>(mDimension), GL_TEXTURE_WRAP_R, static_cast<GLenum>(mWrapMode));
  }

  void Texture::SetMappingType(const std::string& type) { mMappingType = type; }

  void Texture::SetTextureFormet(const TextureFormat format) { mTextureFormat = format; }

  TextureDimension Texture::GetDimension() const { return mDimension; }
}  // namespace Theodore
