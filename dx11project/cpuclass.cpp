#include "cpuclass.h"

CpuClass::CpuClass(){
}

CpuClass::CpuClass(const CpuClass& other){
}

CpuClass::~CpuClass(){
}

void CpuClass::Initialize(){
	PDH_STATUS status;

	//init flag indicative of access to read system cpu usage
	m_canReadCpu = true;

	//create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS) m_canReadCpu = false;
	

	//set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS) m_canReadCpu = false;
	
	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}


void CpuClass::Shutdown(){
	if (m_canReadCpu){
		PdhCloseQuery(m_queryHandle);
	}
}


void CpuClass::Frame(){
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu){
		if ((m_lastSampleTime + 1000) < GetTickCount()){
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}
}


int CpuClass::GetCpuPercentage(){
	int usage;

	if (m_canReadCpu){
		usage = (int)m_cpuUsage;
	} else {
		usage = 0;
	}

	return usage;
}