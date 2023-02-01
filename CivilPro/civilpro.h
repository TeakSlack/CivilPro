#pragma once

#include "device.h"
#include "tl_programmer.h"

// High level I/O
class CivilPro
{
public:
	CivilPro();

private:
	TLProgrammer programmer;
};