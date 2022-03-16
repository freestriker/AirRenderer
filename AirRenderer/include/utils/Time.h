#include <chrono>

class Time
{
public:
	std::chrono::system_clock::time_point launchTime;
	std::chrono::system_clock::time_point preTime;
	std::chrono::system_clock::time_point curTime;
	void Launch()
	{
		launchTime = std::chrono::system_clock::now();
		preTime = launchTime;
		curTime = launchTime;
	}
	void Refresh()
	{
		preTime = curTime;
		curTime = std::chrono::system_clock::now();
	}
	double GetDeltaDuration()
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - preTime);
		return double(duration.count()) / 1000000000;
	}
	double GetLaunchDuration()
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - launchTime);
		return double(duration.count()) / 1000000000;
	}
};