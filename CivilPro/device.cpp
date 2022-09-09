#include <iostream>
#include <string>

#include "device.h"

// This is probably the worst function I've ever written, it's held together by literal black magic
const char* UsbDevice::GetDevicePath()
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

	// pt 2, actually get the device path

	unsigned long requiredLength = 0;

	SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL); // this is expected to return FALSE

	SP_DEVICE_INTERFACE_DETAIL_DATA *detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)operator new(requiredLength); // the windows api is so poorly designed

	if (detailData == NULL) // null checks null checks!!
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

	// this gives the same vibes as bullying george bush
	m_DeviceHandle = CreateFile(devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (!WinUsb_Initialize(m_DeviceHandle, &m_InterfaceHandle))
	{
		CloseHandle(m_DeviceHandle);
		std::cerr << "Could not initialize WinUsb device!" << std::endl;
	}

	m_DevicePresent = true;

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
	CloseHandle(m_DeviceHandle);
}

void UsbDevice::QueryEndpoints()
{
	// get # of endpoints
	USB_INTERFACE_DESCRIPTOR interfaceDesc;
	if (!WinUsb_QueryInterfaceSettings(m_InterfaceHandle, 0, &interfaceDesc)) return;

	WINUSB_PIPE_INFORMATION pipeInfo;

	for (int i = 0; i < interfaceDesc.bNumEndpoints; i++) // for TL866II+, every endpoint will be bulk. Index is i-1
	{
		if (!WinUsb_QueryPipe(m_InterfaceHandle, 0, i, &pipeInfo)) // probably redundant, used in development of application
			return;
		
		if (USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId))
			std::cout << "BULK IN PIPE ID: " << (UINT)pipeInfo.PipeId << " ENDPOINT INDEX: " << i << std::endl;
		else if (USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId))
			std::cout << "BULK OUT PIPE ID: " << (UINT)pipeInfo.PipeId << " ENDPOINT INDEX: " << i << std::endl;
	}
}

template <typename T>
void UsbDevice::Write(T* buffer, unsigned long size, uint8_t endpoint)
{
	if (m_DeviceHandle == INVALID_HANDLE_VALUE) return;
	if (!m_DevicePresent) return;

	ULONG bytesWritten = 0;

	if (!WinUsb_WritePipe(m_InterfaceHandle, endpoint, buffer, size, &bytesWritten, NULL))
		std::cout << "USB write failed!" << std::endl;

	if (m_Verbose)
		std::cout << bytesWritten << " bytes written!" << std::endl;
}

// Endpoint should be given as 0x01, 0x02, or 0x03. It is converted to this format automatically
template <typename T>
void UsbDevice::Read(T* buffer, unsigned long size, uint8_t endpoint)
{
	if (m_DeviceHandle == INVALID_HANDLE_VALUE) return;
	if (!m_DevicePresent) return;

	ULONG bytesRead = 0;

	if(!WinUsb_ReadPipe(m_InterfaceHandle, 0x80 | endpoint, buffer, size, &bytesRead, NULL))
		std::cout << "USB read failed!" << std::endl;

	if (m_Verbose)
		std::cout << bytesRead << " bytes read!" << std::endl;
}
