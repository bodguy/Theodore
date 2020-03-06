// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Time_h
#define Time_h

#include <chrono>
#include <ctime>
#include <string>

namespace Theodore {
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::duration<float, std::milli> milli;
  typedef std::chrono::time_point<Clock> TimePoint;

  class Time {
  public:
    Time();

    static void Reset();
    void Update();

    static float DeltaTime();
    static float DeltaTimeUnscaled();
    static float ElapsedTime();
    static float ElapsedTimeUnscaled();
    static float FixedDeltaTime();
    static float FixedDeltaTimeUnscaled();

    static int FPS();
    static void SetTimeScale(float value);
    static std::string GetDateTime();
    static TimePoint GetTime();
    static float GetInterval(TimePoint start, TimePoint end);

  private:
    static Time* instance;

    TimePoint startTime;
    TimePoint currentTime;
    float accumulator;  // for internal use
    int frameCounter;   // for internal use
    int frameRate;
    float timeScale;

    float deltaTime;
    float unscaledDeltaTime;
    float elapsedTime;
    float unscaledTime;
    float fixedDeltaTime;
    float unscaledFixedDeltaTime;
  };
}  // namespace Theodore

#endif /* Time_h */
