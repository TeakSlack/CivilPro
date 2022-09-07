#include <iostream>
#include <strsafe.h>

#include "device.h"

// This is probably the worst function I've ever written, it's held together by literal black magic
const char* GetDevicePath()
{
	/* 
	okay the following is self explanitory but i have a funny story! 
	i spent at least one hour debugging just this one line because everything after it fucked up.
	it did NOT return an error but the programmer would never be found, even with it plugged in.
	i concluded the issue was the guid which i totally didnt just copy from minipro's source code.
	well after going into device manager and finding the class guid for the programmer myself, i put that in.
	guess what? it didn't work! big shocker.
	after fucking around for far too long i tried putting the original guid back in and it worked.
	god damn i hate c++
	*/
	HDEVINFO deviceInfo = SetupDiGetClassDevs(&TL866IIPLUS_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	
	if (deviceInfo == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Fatal error in SetupDi" << std::endl;
		return NULL;
	}

	SP_DEVICE_INTERFACE_DATA interfaceData;
	interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// have a look at https://pixcl.com/oldsite/SetupDiEnumInterfaces-Fail.htm
	if (!SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &TL866IIPLUS_GUID, 0, &interfaceData))
	{
		std::cerr << "No devices found! Please plug in programmer and make sure connector is secured." << std::endl;
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return NULL;
	}

	unsigned long requiredLength = 0;

	SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL); // this is expected to return FALSE

	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredLength);

	if (!detailData)
	{
		free(detailData);
		SetupDiDestroyDeviceInfoList(deviceInfo);
	}

	// shut up msvc this isn't a null pointer
	detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	unsigned long length = requiredLength;

	if (!SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, detailData, length, &requiredLength, NULL))
	{
		std::cerr << "Fatal error in SetupDiGetDeviceInterfaceDetail: " << HRESULT_FROM_WIN32(GetLastError()) << std::endl;
		SetupDiDestroyDeviceInfoList(deviceInfo);
		free(detailData);
		return NULL;

	}

	return _strdup(detailData->DevicePath);

	free(detailData);
	SetupDiDestroyDeviceInfoList(deviceInfo);
}