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
	static double mFrequency; // CPU�߾��ȼ�����Ƶ�ʣ���λ��/��
	static double mDurationSecond; // CPU����һ�ε����ڣ���λ��
	static double mDurationMillisecond; // CPU����һ�ε����ڣ���λ����

	double mStart;
};

PE_END

#endif