#pragma once

#include <chrono>
#include <thread>


void Sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

class Tick
{
public:
	Tick()
	{
		start = std::chrono::high_resolution_clock::now();

	}
	bool Elapsed(double ms)
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>( now - start );

		if (elapsedTime.count() >= ms)
		{
			start = std::chrono::high_resolution_clock::now();
			return true;
		}

		return false;
	}
private:
	std::chrono::high_resolution_clock::time_point start;
};