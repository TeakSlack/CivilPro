#include "civilpro.h"
#include "device.h"

#include <iostream>

CivilPro::CivilPro()
{
	ProgrammerInfo info = programmer.GetProgrammerInfo();

	DeviceInfo *dev_inf = programmer.GetDevice("atmega32");

	if (dev_inf == nullptr)
	{
		std::cout << "Device not found!" << std::endl;
	}
}