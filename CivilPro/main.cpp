#include <iostream>

#include "device.h"

typedef struct minipro_report_info
{
	uint8_t  echo;
	uint8_t  device_status;
	uint16_t report_size;
	uint8_t  firmware_version_minor;
	uint8_t  firmware_version_major;
	uint16_t device_version; // changed from byte to word.
	uint8_t  device_code[8];
	uint8_t  serial_number[24];
	uint8_t  hardware_version;
};

int main()
{
	UsbDevice* usbDevice = new UsbDevice;

	uint8_t buffer = 0x00;
	usbDevice->Write(&buffer, 5, 0x01);

	uint8_t msg[sizeof(minipro_report_info)] = {0};

	usbDevice->Read(&msg, sizeof(msg), 0x81);

	minipro_report_info deviceInfo;
	std::memcpy(&deviceInfo, msg, sizeof(msg));

	delete usbDevice;
	return 0;
}