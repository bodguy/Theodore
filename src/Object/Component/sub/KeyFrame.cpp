// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "KeyFrame.h"

namespace Theodore {
  KeyFrame::KeyFrame(float time, float value) : mTime(time), mValue(value) {}

  KeyFrame::~KeyFrame() {}
}  // namespace Theodore