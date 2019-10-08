// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef RenderBuffer_h
#define RenderBuffer_h

namespace Theodore {
  class RenderBuffer {
  public:
    RenderBuffer();
    ~RenderBuffer();

    unsigned int GetRenderBufferID() const;
    void Storage(unsigned int width, unsigned int height);

  private:
    unsigned int mRenderBufferID;
  };
}  // namespace Theodore

#endif /* RenderBuffer_h */
