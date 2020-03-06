// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "MSAATexture2D.h"

namespace Theodore {
  MSAATexture2D::MSAATexture2D() {
		assetType = AssetType::TextureType;
		textureDimension = TextureDimension::MSAATex2D;
  }

  MSAATexture2D::~MSAATexture2D() {}

  bool MSAATexture2D::LoadMultiSampleTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned int sample) {
    glGenTextures(1, &textureId);
    glBindTexture(static_cast<GLenum>(textureDimension), textureId);
		width = width;
		height = height;

    switch (format) {
      case TextureFormat::RGBA32:
        glTexImage2DMultisample(static_cast<GLenum>(textureDimension), sample, GL_RGBA8, width, height, GL_TRUE);
        break;
      case TextureFormat::RGB24:
        glTexImage2DMultisample(static_cast<GLenum>(textureDimension), sample, GL_RGB, width, height, GL_TRUE);
        break;
      case TextureFormat::Red8:
        glTexImage2DMultisample(static_cast<GLenum>(textureDimension), sample, GL_RED, width, height, GL_TRUE);
        break;
      case TextureFormat::Green8:
        glTexImage2DMultisample(static_cast<GLenum>(textureDimension), sample, GL_GREEN, width, height, GL_TRUE);
        break;
      case TextureFormat::Blue8:
        glTexImage2DMultisample(static_cast<GLenum>(textureDimension), sample, GL_BLUE, width, height, GL_TRUE);
        break;
      default:
        return false;
    }

    glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

    return true;
  }
}  // namespace Theodore
