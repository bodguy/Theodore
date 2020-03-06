// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef KeyFrame_h
#define KeyFrame_h

namespace Theodore {
  class KeyFrame {
  public:
    KeyFrame(float time, float value);
    ~KeyFrame();

  private:
    float timeFrame;
    float value;
  };
} // namespace Theodore

#endif /* KeyFrame_h */