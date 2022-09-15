#include "civilpro.h"

#include <iostream>

CivilPro::CivilPro()
{
	if (!device.IsDevicePresent())
	{
		std::cout << "Programmer not found! Please make sure the device is plugged in and the connector is secured." << std::endl;
	}
}