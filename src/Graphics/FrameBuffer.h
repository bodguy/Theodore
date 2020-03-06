// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef FrameBuffer_h
#define FrameBuffer_h

#include <map>

#include "Enumeration.h"

namespace Theodore {
  class Texture2D;
  class RenderBuffer;
  class FrameBuffer {
  public:
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer();

    bool Create(bool writable);

    bool AttachTexture(Texture2D* tex, Attachment attach);
    Texture2D* GetRenderTexture(Attachment attach);
    void SetRenderBuffer(RenderBuffer* buffer);

    bool IsCreated() const;
    unsigned int GetFrameBufferID() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    Texture2D* GetTexture(Attachment attach);
    void GetBufferInfo();

    void SetDimension(unsigned int w, unsigned int h);

  private:
    unsigned int frameBufferId;
    unsigned int width;
    unsigned int height;
    std::map<Attachment, Texture2D*> textures;
    RenderBuffer* renderBuffer;
    bool isCreated;
  };
}  // namespace Theodore

#endif /* FrameBuffer_h */
