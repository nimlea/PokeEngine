#pragma once
#ifndef _ENGINETIMER_H_
#define _ENGINETIMER_H_

#include "..\Common\PEMacro.h"

PE_BEGIN

class EngineTimer
{
public:
	EngineTimer();
	
	void Reset();
	float GetSeconds() const;
	float GetMilliseconds() const;

private:
	static double mFrequency; // CPU高精度计数器频率，单位次/秒
	static double mDurationSecond; // CPU计数一次的周期，单位秒
	static double mDurationMillisecond; // CPU计数一次的周期，单位毫秒

	double mStart;
};

PE_END

#endif