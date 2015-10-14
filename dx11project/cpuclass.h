#ifndef _CPUCLASS_H_
#define _CPUCLASS_H_

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class CpuClass{
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();	
};

#endif