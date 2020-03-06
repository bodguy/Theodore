// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Time.h"
#include "Helper/StringUtil.h"

namespace Theodore {
  Time* Time::instance = nullptr;
  Time::Time() : accumulator(0.f), frameCounter(0), frameRate(0), deltaTime(0.f), unscaledDeltaTime(0.f), elapsedTime(0.f), unscaledTime(0.f), fixedDeltaTime(0.f) {
		unscaledFixedDeltaTime = 20.f * 0.001f;
    instance = this;
    Reset();
  }

  void Time::Reset() {
    instance->startTime = Clock::now();
    instance->timeScale = 1.f;
  }

  void Time::Update() {
		currentTime = Clock::now();

		unscaledDeltaTime = std::chrono::duration_cast<milli>(currentTime - startTime).count() * 0.001f;
		deltaTime = unscaledDeltaTime * timeScale;

		unscaledTime += unscaledDeltaTime;
		elapsedTime += deltaTime;

		fixedDeltaTime = unscaledFixedDeltaTime * timeScale;

		startTime = currentTime;
    frameCounter++;
		accumulator += unscaledDeltaTime;  // accumulate
    if (accumulator > 1.0f) {           // elapsed after 1 sec
      frameRate = frameCounter;
			frameCounter = 0;
			accumulator = 0.0f;
    }
  }

  float Time::DeltaTime() { return instance->deltaTime; }

  float Time::DeltaTimeUnscaled() { return instance->unscaledDeltaTime; }

  float Time::ElapsedTime() { return instance->elapsedTime; }

  float Time::ElapsedTimeUnscaled() { return instance->unscaledTime; }

  float Time::FixedDeltaTime() { return instance->fixedDeltaTime; }

  float Time::FixedDeltaTimeUnscaled() { return instance->unscaledFixedDeltaTime; }

  int Time::FPS() { return instance->frameRate; }

  void Time::SetTimeScale(float value) { instance->timeScale = value; }

  std::string Time::GetDateTime() {
    std::time_t now = std::time(nullptr);
    return StringUtil::ToDateFormat(std::localtime(&now));
  }

  TimePoint Time::GetTime() { return Clock::now(); }

  float Time::GetInterval(TimePoint start, TimePoint end) { return std::chrono::duration_cast<milli>(end - start).count(); }
}  // namespace Theodore
