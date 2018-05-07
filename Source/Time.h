#ifndef Time_h
#define Time_h

#include <chrono>
#include <string>
#include <ctime>

namespace Quark {
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

    private:
        typedef std::chrono::high_resolution_clock Clock;
        typedef std::chrono::duration<float, std::milli> milli;
        
        static Time* instance;
        
        std::chrono::time_point<Clock> start;
        std::chrono::time_point<Clock> currentTime;
        float elapsedtime;
        float accumulation;
        float realtimeSinceStartup;
        int fps_counter;
        int fps;
        float scale;
    };
}

#endif /* Time_h */
