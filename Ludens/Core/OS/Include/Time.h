#pragma once

#include <chrono>

namespace LD {

	class DeltaTime
	{
	public:
		DeltaTime() = delete;
		DeltaTime(double seconds) : mSeconds(seconds) {}
		~DeltaTime() {}

		inline operator double() const { return mSeconds; }
		inline double GetSeconds() const { return mSeconds; }
		inline double GetMilliSeconds() const { return mSeconds * 1000.0; }

	private:
		double mSeconds;
	};

    class Timer
    {
    public:
        void Start();
        void Stop();

        double GetMilliseconds();
        double GetSeconds() { return GetMilliseconds() / 1000.0; }

    private:
        std::chrono::time_point<std::chrono::system_clock> mStartTime;
        std::chrono::time_point<std::chrono::system_clock> mEndTime;
        bool mIsRunning = false;
    };

    class ScopeTimer
    {
    public:
        ScopeTimer() = delete;
        ScopeTimer(double* milliSeconds)
            : mMilliSeconds(milliSeconds)
        {
            mTimer.Start();
        }

        ~ScopeTimer()
        {
            mTimer.Stop();

            if (mMilliSeconds)
                *mMilliSeconds = mTimer.GetMilliseconds();
        }

    private:
        Timer mTimer;
        double* mMilliSeconds;
    };

} // namespace LD