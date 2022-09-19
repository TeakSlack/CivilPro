#pragma once

#include "device.h"

constexpr int command_getsysteminfo = 0x00;

struct ProgrammerInfo
{
	uint8_t  echo;
	uint8_t  device_status;
	uint16_t report_size;
	uint8_t  firmware_version_minor;
	uint8_t  firmware_version_major;
	uint16_t device_version;
	uint8_t  device_code[8];
	uint8_t  serial_number[24];
	uint8_t  hardware_version;
};

class TLProgrammer
{
public:
	ProgrammerInfo GetProgrammerInfo();
	void SetVerbose()
	{
		m_Device.SetVerboseWrite();
	}
private:
	UsbDevice m_Device;
};