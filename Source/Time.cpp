#include "Time.h"
#include "StringUtil.h"

namespace Quark {
    Time* Time::instance = nullptr;
    Time::Time() : elapsedtime(0.f), accumulation(0.f), realtimeSinceStartup(0.f), fps_counter(0), fps(0) {
        instance = this;
        Reset();
    }
    
    void Time::Reset() {
        instance->start = Clock::now();
        instance->scale = 1 / 1000.f;
    }
    
    void Time::Update() {
        currentTime = Clock::now();
        elapsedtime = std::chrono::duration_cast<milli>(currentTime - start).count() * scale;
        start = currentTime;
        fps_counter++;
        accumulation += elapsedtime; // accumulate
        realtimeSinceStartup += elapsedtime;
        if (accumulation > 1.0) { // elapsed after 1 sec
            fps = fps_counter;
            fps_counter = 0;
            accumulation = 0.0;
        }
    }
    
    float Time::DeltaTime() {
        return instance->elapsedtime;
    }
    
    float Time::ElapsedTime() {
        return instance->realtimeSinceStartup;
    }
    
    int Time::FPS() {
        return instance->fps;
    }
    
    void Time::SetTimeScale(float value) {
        instance->scale = value / 1000.f;
    }

	std::string Time::GetDateTime() {
		std::time_t now = std::time(nullptr);
		return StringUtil::DateToUTCString(std::localtime(&now));
	}
}
