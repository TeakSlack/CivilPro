#pragma once

#include "device.h"

constexpr int command_getsysteminfo = 0x00;
constexpr int command_begintransaction = 0x03;

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

typedef struct Fuses
{
	const char* name;
	uint8_t minipro_cmd;
	uint8_t length;
	uint32_t offset;
} Fuses;

extern Fuses avr_fuses[];
extern Fuses avr2_fuses[];
extern Fuses avr3_fuses[];
extern Fuses pic_fuses[];
extern Fuses pic2_fuses[];

typedef struct DeviceInfo
{
	const char* name;
	uint8_t protocol_id;
	uint8_t variant;
	uint32_t read_buffer_size;
	uint32_t write_buffer_size;
	uint32_t code_memory_size; // Presenting for every device
	uint32_t data_memory_size;
	size_t data_memory2_size;
	uint32_t chip_id; // A vendor-specific chip ID (i.e. 0x1E9502 for ATMEGA48)
	uint8_t chip_id_bytes_count;
	uint16_t opts1;
	uint16_t opts2;
	uint32_t opts3;
	uint32_t opts4;
	uint32_t package_details; // pins count or image ID for some devices
	uint16_t write_unlock;
	uint8_t	id_shift;//PIC controllers device ID have a variable bitfield Revision number
	Fuses* fuses; // Configuration bytes that's presenting in some architectures
} DeviceInfo;

typedef struct BeginTransactionPayload
{
	uint8_t command;
	uint8_t protocol_id;
	uint8_t variant;
	uint8_t icsp;
	uint8_t empty1;
	uint16_t opts1;
	uint8_t empty2;
	uint32_t data_memory_size;
	uint16_t opts2;
	uint32_t opts3;
	size_t data_memory2_size;
	uint32_t code_memory_size;
	uint8_t empty3[20];
	uint32_t package_details;
} BeginTransactionPayload;

class TLProgrammer
{
public:
	TLProgrammer();

	ProgrammerInfo GetProgrammerInfo();
	DeviceInfo* GetDevice(const char* name);

	inline void SetVerbose()
	{
		m_Programmer.SetVerboseWrite();
	}

	void BeginTransaction(DeviceInfo *device);
private:

	UsbDevice m_Programmer;
};