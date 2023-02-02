#pragma once

#include "device.h"
#include "tl_programmer.h"

// High level I/O
class CivilPro
{
public:
	CivilPro();

	void Read();
private:
	TLProgrammer m_TLProgrammer;
	DeviceInfo* m_Device;
};