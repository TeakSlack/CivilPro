#pragma once

#include "device.h"

// Central state managment and command processing
class CivilPro
{
public:
	CivilPro();

private:
	UsbDevice device;
};