#include <iostream>

#include "device.h"

const char* UsbDevice::GetDevicePath() // See https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/using-winusb-api-to-communicate-with-a-usb-device
{
	HDEVINFO deviceInfo = SetupDiGetClassDevs(&TL866IIPLUS_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	
	if (deviceInfo == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Fatal error in SetupDi" << std::endl;
		return NULL;
	}

	SP_DEVICE_INTERFACE_DATA interfaceData;
	interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// See https://pixcl.com/oldsite/SetupDiEnumInterfaces-Fail.htm
	if (!SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &TL866IIPLUS_GUID, 0, &interfaceData))
	{
		m_ProgrammerPresent = false;
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return NULL;
	}

	// Acquire the device path

	unsigned long requiredLength = 0;

	SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL); // this is expected to return FALSE

	SP_DEVICE_INTERFACE_DETAIL_DATA *detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)operator new(requiredLength); // the windows api is so poorly designed

	if (detailData == NULL)
	{
		free(detailData);
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return NULL;
	}

	
	detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	unsigned long length = requiredLength;

	if (!SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, detailData, length, &requiredLength, NULL))
	{
		std::cerr << "Fatal error in SetupDiGetDeviceInterfaceDetail" << std::endl;
		SetupDiDestroyDeviceInfoList(deviceInfo);
		free(detailData);
		return NULL;

	}

	SetupDiDestroyDeviceInfoList(deviceInfo);

	return detailData->DevicePath;
}

void UsbDevice::SetupUsb()
{
	const char* devicePath = GetDevicePath();
	if (devicePath == NULL)
		return;

	// I love WinApi!
	m_ProgrammerHandle = CreateFile(devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (!WinUsb_Initialize(m_ProgrammerHandle, &m_InterfaceHandle))
	{
		CloseHandle(m_ProgrammerHandle);
		std::cerr << "Could not initialize WinUsb device!" << std::endl;
		exit(-1);
	}

	m_ProgrammerPresent = true;

	uint8_t value = 1;
	WinUsb_SetPipePolicy(m_InterfaceHandle, 0x81, AUTO_FLUSH, 1, &value);
	WinUsb_SetPipePolicy(m_InterfaceHandle, 0x82, AUTO_FLUSH, 1, &value);
	WinUsb_SetPipePolicy(m_InterfaceHandle, 0x83, AUTO_FLUSH, 1, &value);
}

UsbDevice::UsbDevice(bool verbose)
	: m_Verbose(verbose)
{
	SetupUsb();
}

UsbDevice::~UsbDevice()
{
	WinUsb_Free(m_InterfaceHandle);
	CloseHandle(m_ProgrammerHandle);
}

void UsbDevice::QueryEndpoints()
{
	// get # of endpoints
	USB_INTERFACE_DESCRIPTOR interfaceDesc;
	if (!WinUsb_QueryInterfaceSettings(m_InterfaceHandle, 0, &interfaceDesc)) return;

	WINUSB_PIPE_INFORMATION pipeInfo;

	for (int i = 0; i < interfaceDesc.bNumEndpoints; i++) // for TL866II+, every endpoint will be bulk. Index is i-1
	{
		if (!WinUsb_QueryPipe(m_InterfaceHandle, 0, i, &pipeInfo))
			return;
		
		if (USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId))
			std::cout << "BULK IN PIPE ID: " << (UINT)pipeInfo.PipeId << " ENDPOINT INDEX: " << i << std::endl;
		else if (USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId))
			std::cout << "BULK OUT PIPE ID: " << (UINT)pipeInfo.PipeId << " ENDPOINT INDEX: " << i << std::endl;
	}
}