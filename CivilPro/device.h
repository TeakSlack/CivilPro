#pragma once

#include <Windows.h>
#include <winusb.h>
#include <SetupAPI.h>

DEFINE_GUID(TL866IIPLUS_GUID, 0xE7E8BA13, 0x2A81, 0x446E, 0xA1, 0x1E, 0x72, 0x39, 0x8F, 0xBD, 0xA8, 0x2F);

// UsbDevice manages I/O for the TL866II+
class UsbDevice
{
public:
    // Find TL866II+ programmer and create handle for it.
    UsbDevice(bool verbose = false);
    // Clean up
	~UsbDevice();
    
    // Write to programmer
    template <typename T>
    void Write(T* buffer, unsigned long size, int endpoint);
    // Read from programmer
    template <typename T>
    void Read(T *buffer, unsigned long size, int endpoint);
    // Check if usb device is present
    bool IsDevicePresent() const
    {
        return m_DevicePresent;
    }
    void SetVerboseWrite()
    {
        m_Verbose = !m_Verbose;
    }
private:
    // Get path for TL866II+, called in constructor
    const char* GetDevicePath();
    // Dynamically find USB endpoints
    void QueryEndpoints();
    void SetupUsb();

    // Internal member data
    bool m_Verbose;
    bool m_DevicePresent;
    WINUSB_INTERFACE_HANDLE m_InterfaceHandle;
    HANDLE m_DeviceHandle;
};