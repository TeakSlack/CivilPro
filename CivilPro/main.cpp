#include <iostream>

#include "device.h"

int main()
{
	UsbDevice *usbDevice = new UsbDevice;
	
	delete usbDevice;
	return 0;
}