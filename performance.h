#pragma once

class PerformanceCounter
{
private: // copy data
	LARGE_INTEGER li_start, li_end, li_freq;
	__int64	      i64_start, i64_end, i64_freq;

public: // query performance on start
	static const int AUTO_START = 1;
	double actual_time;

	PerformanceCounter(int Start = 0)
	{
		QueryPerformanceFrequency(&li_freq);
		memcpy(&i64_freq, &li_freq, sizeof (LARGE_INTEGER));
		if (Start) start();
	}

	void start()
	{
		QueryPerformanceCounter(&li_start);
	}

	double stop()
	{
		QueryPerformanceCounter(&li_end);
		memcpy(&i64_start, &li_start, sizeof (LARGE_INTEGER));
		memcpy(&i64_end,   &li_end,   sizeof (LARGE_INTEGER));
		actual_time = (i64_end - i64_start) / (double)i64_freq;
		return actual_time;
	}
};