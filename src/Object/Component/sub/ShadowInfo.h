// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef ShadowInfo_h
#define ShadowInfo_h

namespace Theodore {
  class FrameBuffer;
  class Pipeline;
  class ShadowInfo {
  public:
    ShadowInfo();
    ~ShadowInfo();

  private:
    FrameBuffer* frame;
    Pipeline* shadowProgram;
  };
}  // namespace Theodore

#endif /* ShadowInfo_h */