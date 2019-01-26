#include "MSAATexture2D.h"

namespace Theodore {
  MSAATexture2D::MSAATexture2D() {
    mType = AssetType::TextureType;
    mDimension = TextureDimension::MSAATex2D;
  }

  MSAATexture2D::~MSAATexture2D() {}

  bool MSAATexture2D::LoadMultiSampleTexture(unsigned int width, unsigned int height,
                                             TextureFormat format, unsigned int sample) {
    glGenTextures(1, &mTextureID);
    glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
    mWidth = width;
    mHeight = height;

    switch (format) {
    case TextureFormat::RGBA32:
      glTexImage2DMultisample(static_cast<GLenum>(mDimension), sample, GL_RGBA8, mWidth, mHeight,
                              GL_TRUE);
      break;
    case TextureFormat::RGB24:
      glTexImage2DMultisample(static_cast<GLenum>(mDimension), sample, GL_RGB, mWidth, mHeight,
                              GL_TRUE);
      break;
    case TextureFormat::Red8:
      glTexImage2DMultisample(static_cast<GLenum>(mDimension), sample, GL_RED, mWidth, mHeight,
                              GL_TRUE);
      break;
    case TextureFormat::Green8:
      glTexImage2DMultisample(static_cast<GLenum>(mDimension), sample, GL_GREEN, mWidth, mHeight,
                              GL_TRUE);
      break;
    case TextureFormat::Blue8:
      glTexImage2DMultisample(static_cast<GLenum>(mDimension), sample, GL_BLUE, mWidth, mHeight,
                              GL_TRUE);
      break;
    }

    glBindTexture(static_cast<GLenum>(mDimension), NULL);

    return true;
  }
}
