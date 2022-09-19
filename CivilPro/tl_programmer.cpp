#include <iostream>

#include "tl_programmer.h"

ProgrammerInfo TLProgrammer::GetProgrammerInfo()
{
	if (!m_Device.IsDevicePresent())
	{
		std::cerr << "No programmer found!" << std::endl;
		exit(0);
	}

	ProgrammerInfo info;

	m_Device.Write<const int>(&command_getsysteminfo, sizeof(command_getsysteminfo), 0x01);
	m_Device.Read<ProgrammerInfo>(&info, sizeof(info), 0x01);

	return info;
}