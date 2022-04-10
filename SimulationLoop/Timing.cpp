#include "Timing.h"
#include <Windows.h>
#if !_WIN32
#include <chrono>
using namespace std::chrono;
#endif
namespace
{
#if _WIN32
	LARGE_INTEGER sStartTime = { 0 };
#else
	high_resolution_clock::time_point sStartTime;
#endif
}
timing timing::TheTime;
std::mutex timing::_time_mutex;
std::mutex timing::_dtime_mutex;
timing::timing()
{
	
	set_desired_frame_time(0.03333333f);
#if _WIN32
	LARGE_INTEGER perfFreq;
	QueryPerformanceFrequency(&perfFreq);
	count_duration_ = 1.0 / perfFreq.QuadPart;

	QueryPerformanceCounter(&sStartTime);

	last_frame_start_time_ = get_time();
#else
	sStartTime = high_resolution_clock::now();
#endif
	

}
timing::~timing()
{

}

void timing::update()
{
	std::lock_guard<std::mutex>lock(_time_mutex);
	double currentTime = get_time();

	dt_ = (float)(currentTime - last_frame_start_time_);

	//frame lock at 30fps
	while (dt_ < desired_frame_time_)
	{
		currentTime = get_time();

		dt_ = (float)(currentTime - last_frame_start_time_);
	}

	//set the delta time to the desired frame time, to try to account
	//for potential slight fluctuations that may occur when exiting the loop
	//this also will handle the frame time not going crazy if spammed with events
	dt_ = desired_frame_time_;

	last_frame_start_time_ = currentTime;
	current_frame_start_time_ = static_cast<float> (last_frame_start_time_);
}

double timing::get_time()const
{
	//std::lock_guard<std::mutex>lock(_time_mutex);
#if _WIN32
	LARGE_INTEGER curTime, timeSinceStart;
	QueryPerformanceCounter(&curTime);

	timeSinceStart.QuadPart = curTime.QuadPart - sStartTime.QuadPart;

	return timeSinceStart.QuadPart * count_duration_;
#else
	auto now = high_resolution_clock::now();
	auto ms = duration_cast<milliseconds>(now - sStartTime).count();
	//a little uncool to then convert into a double just to go back, but oh well.
	return static_cast<double>(ms) / 1000;
#endif
}