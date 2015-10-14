#include "timerclass.h"

TimerClass::TimerClass(){
}


TimerClass::TimerClass(const TimerClass& other){
}


TimerClass::~TimerClass(){
}


bool TimerClass::Initialize()
{
	//check if system supports performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0) return false;

	//frequency counter ticks per millisecond
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}


void TimerClass::Frame(){
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;
	m_startTime = currentTime;
}


float TimerClass::GetTime(){
	return m_frameTime;
}