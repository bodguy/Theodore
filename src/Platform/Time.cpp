#include "Time.h"
#include "../Helper/StringUtil.h"

namespace Theodore {
  Time* Time::instance = nullptr;
  Time::Time()
      : mAccumulator(0.f), mFrameCounter(0), mFrameRate(0), mDeltaTime(0.f),
        mUnscaledDeltaTime(0.f), mTime(0.f), mUnscaledTime(0.f), mFixedDeltaTime(0.f) {
    mUnscaledFixedDeltaTime = 20.f * 0.001f;
    instance = this;
    Reset();
  }

  void Time::Reset() {
    instance->mStart = Clock::now();
    instance->mTimeScale = 1.f;
  }

  void Time::Update() {
    mCurrentTime = Clock::now();

    mUnscaledDeltaTime = std::chrono::duration_cast<milli>(mCurrentTime - mStart).count() * 0.001f;
    mDeltaTime = mUnscaledDeltaTime * mTimeScale;

    mUnscaledTime += mUnscaledDeltaTime;
    mTime += mDeltaTime;

    mFixedDeltaTime = mUnscaledFixedDeltaTime * mTimeScale;

    mStart = mCurrentTime;
    mFrameCounter++;
    mAccumulator += mUnscaledDeltaTime; // accumulate
    if (mAccumulator > 1.0f) {          // elapsed after 1 sec
      mFrameRate = mFrameCounter;
      mFrameCounter = 0;
      mAccumulator = 0.0f;
    }
  }

  float Time::DeltaTime() { return instance->mDeltaTime; }

  float Time::DeltaTimeUnscaled() { return instance->mUnscaledDeltaTime; }

  float Time::ElapsedTime() { return instance->mTime; }

  float Time::ElapsedTimeUnscaled() { return instance->mUnscaledTime; }

  float Time::FixedDeltaTime() { return instance->mFixedDeltaTime; }

  float Time::FixedDeltaTimeUnscaled() { return instance->mUnscaledFixedDeltaTime; }

  int Time::FPS() { return instance->mFrameRate; }

  void Time::SetTimeScale(float value) { instance->mTimeScale = value; }

  std::string Time::GetDateTime() {
    std::time_t now = std::time(nullptr);
    return StringUtil::DateToUTCString(std::localtime(&now));
  }

  TimePoint Time::GetTime() { return Clock::now(); }

  float Time::GetInterval(TimePoint start, TimePoint end) {
    return std::chrono::duration_cast<milli>(end - start).count();
  }
}
