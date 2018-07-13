#ifndef Time_h
#define Time_h

#include <chrono>
#include <string>
#include <ctime>

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
        static float ElapsedTime();
        static int FPS();
        static void SetTimeScale(float value);
		static std::string GetDateTime();
		static TimePoint GetTime();
		static float GetInterval(TimePoint start, TimePoint end);

    private:        
        static Time* instance;
        
		TimePoint start;
		TimePoint currentTime;
        float elapsedtime;
        float accumulation;
        float realtimeSinceStartup;
        int fps_counter;
        int fps;
        float scale;
    };
}

#endif /* Time_h */
