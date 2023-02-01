#include "civilpro.h"
#include "device.h"

#include <iostream>

CivilPro::CivilPro()
{
	ProgrammerInfo info = programmer.GetProgrammerInfo();

	DeviceInfo *device_info = programmer.GetDevice("SST39SF020A");

	if (device_info == nullptr)
	{
		std::cout << "Device not found!" << std::endl;
		exit(0);
	}

	programmer.BeginTransaction(device_info);
}