#include "civilpro.h"
#include "device.h"

#include <iostream>

CivilPro::CivilPro()
{
	//ProgrammerInfo info = programmer.GetProgrammerInfo();

	DeviceInfo *dev_inf = programmer.GetDevice("sst39sf020a");

	if (dev_inf == nullptr)
	{
		std::cout << "Device not found!" << std::endl;
		exit(0);
	}

	programmer.BeginTransaction(dev_inf);
}