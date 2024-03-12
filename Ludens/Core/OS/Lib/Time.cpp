#include "Core/OS/Include/Time.h"

namespace LD {


    void Timer::Start()
    {
	    mStartTime = std::chrono::system_clock::now();
	    mIsRunning = true;
    }

    void Timer::Stop()
    {
	    mEndTime = std::chrono::system_clock::now();
	    mIsRunning = false;
    }

    double Timer::GetMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (mIsRunning)
            endTime = std::chrono::system_clock::now();
        else
            endTime = mEndTime;

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - mStartTime).count();
    }

} // namespace LD