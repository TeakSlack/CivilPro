#pragma once

#include <Windows.h>
#include <initguid.h>
#include <winusb.h>
#include <SetupAPI.h>
#include <guiddef.h>
#include <usbiodef.h>

typedef struct _DEVICE_DATA {
    WINUSB_INTERFACE_HANDLE WinusbInterfaceHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];

} DEVICE_DATA, * PDEVICE_DATA;

DEFINE_GUID(TL866IIPLUS_GUID, 0xE7E8BA13, 0x2A81, 0x446E, 0xA1, 0x1E, 0x72, 0x39, 0x8F, 0xBD, 0xA8, 0x2F);

class UsbDevice
{
public:
	UsbDevice();
	~UsbDevice();

private:
    const char* GetDevicePath();

    DEVICE_DATA deviceData;
};