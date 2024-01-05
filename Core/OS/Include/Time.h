#pragma once

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

} // namespace LD