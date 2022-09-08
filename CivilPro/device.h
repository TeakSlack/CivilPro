#pragma once

#include <Windows.h>
#include <winusb.h>
#include <SetupAPI.h>

DEFINE_GUID(TL866IIPLUS_GUID, 0xE7E8BA13, 0x2A81, 0x446E, 0xA1, 0x1E, 0x72, 0x39, 0x8F, 0xBD, 0xA8, 0x2F);

// Small class gang! A vague description of each function is provided below.
class UsbDevice
{
public:
    // Find TL866II+ programmer and create handle for it.
	UsbDevice();
    // Clean up
	~UsbDevice();
    
    // Write to programmer
    void Write(void* buffer, size_t size, uint8_t endpoint);
    // Read from programmer
    void Read(void *buffer, size_t size, uint8_t endpoint);
private:
    // Get path for TL866II+, called in constructor
    const char* GetDevicePath();
    // Dynamically find USB endpoints
    void QueryEndpoints();

    // Internal member data
    WINUSB_INTERFACE_HANDLE m_InterfaceHandle;
    HANDLE m_DeviceHandle;
};