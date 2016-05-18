#include "EngineTimer.h"
#include <windows.h>
#include "LogSystem.h"

PE_BEGIN

double EngineTimer::mFrequency = 0.0f;
double EngineTimer::mDurationSecond = 0.0f;
double EngineTimer::mDurationMillisecond = 0.0f;

EngineTimer::EngineTimer()
{
	LARGE_INTEGER largeInt;

	// 只查询一次频率值
	if (mFrequency == 0.0f)
	{
		if (!QueryPerformanceFrequency(&largeInt))
		{
			LogSystem::GetInstance().Log("CPU不支持高精度计时器！");
			return;
		}
		mFrequency = double(largeInt.QuadPart);

		mDurationSecond = 1.0f / mFrequency;
		mDurationMillisecond = mDurationSecond * 1000.0f;
	}

	Reset();
}

void EngineTimer::Reset()
{
	LARGE_INTEGER largeInt;
	if (!QueryPerformanceCounter(&largeInt))
	{
		mStart = 0.0f;
		return;
	}
	mStart = double(largeInt.QuadPart);
}

float EngineTimer::GetSeconds() const
{
	LARGE_INTEGER largeInt;
	if (!QueryPerformanceCounter(&largeInt))
	{
		return 0.0f;
	}
	double end = (double)largeInt.QuadPart;
	float s = float((end - mStart) * mDurationSecond);
	return s;
}

float EngineTimer::GetMilliseconds() const
{
	LARGE_INTEGER largeInt;
	if (!QueryPerformanceCounter(&largeInt))
	{
		return 0.0f;
	}
	double end = (double)largeInt.QuadPart;
	float ms = float((end - mStart) * mDurationMillisecond);
	return ms;
}

PE_END