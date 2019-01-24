#include "KeyFrame.h"

namespace Theodore {
  KeyFrame::KeyFrame(float time, float value) : mTime(time), mValue(value) {}

  KeyFrame::~KeyFrame() {}
}