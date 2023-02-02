#include <iostream>
#include <string>

#include "tl_programmer.h"

static DeviceInfo devices[] =
{
#include "infoic2plus_devices.h"
	{.name = NULL},
};

TLProgrammer::TLProgrammer()
{
	if (!m_Programmer.IsDevicePresent())
	{
		std::cout << "No programmer found!" << std::endl;
		exit(0);
	}
}

ProgrammerInfo TLProgrammer::GetProgrammerInfo()
{
	ProgrammerInfo info;

	m_Programmer.Write<const int>(&command_getsysteminfo, sizeof(command_getsysteminfo), 0x01);
	m_Programmer.Read<ProgrammerInfo>(&info, sizeof(info), 0x01);

	return info;
}

DeviceInfo* TLProgrammer::GetDevice(const char* name)
{
	DeviceInfo* device;

	for (device = &devices[0]; device[0].name; device = &device[1])
	{
		if (_stricmp(name, device->name) == 0)
		{
			return device;
		}
	}
	return nullptr;
}

void TLProgrammer::SetDevice(DeviceInfo* info)
{
	m_Device = info;
}

// sends 64 byte structure about the chip being read or written from
void TLProgrammer::BeginTransaction(DeviceInfo* device)
{
	m_Device = device;

	BeginTransactionPayload payload;
	memset(&payload, 0, 64); // make sure to zero out the payload

	payload.command = command_begintransaction;
	payload.protocol_id = device->protocol_id;
	payload.variant = device->variant;
	payload.opts1 = device->opts1;
	payload.data_memory_size = device->data_memory_size;
	payload.opts2 = device->opts2;
	payload.opts3 = device->opts3;
	payload.data_memory2_size = device->data_memory2_size;
	payload.code_memory_size = device->code_memory_size;
	payload.package_details = device->package_details;

	m_Programmer.Write<BeginTransactionPayload>(&payload, 64, 1);

	// TODO: test for parity between this and MiniPro's begin transaction data structure.
}

void TLProgrammer::EndTransaction()
{
	EndTransactionPayload payload;
	memset(&payload, 0, 8);

	payload.command = command_endtransaction;

	m_Programmer.Write<EndTransactionPayload>(&payload, 8, 1);
}

char* TLProgrammer::ReadBlock(unsigned int length, unsigned int address)
{
	ReadBlockPayload payload;
	
	payload.command = command_readcodememory;
	payload.protocol_id = m_Device->protocol_id;
	payload.length = length;
	payload.address = address;

	m_Programmer.Write<ReadBlockPayload>(&payload, 8, 1);

	char* buffer = new char[length];

	if (length < 64)
	{
		m_Programmer.Read(buffer, length, 1);
	}
	else
	{
		uint32_t endpoint_length = length / 2;

		m_Programmer.Read(buffer, endpoint_length, 2);
		m_Programmer.Read(buffer + endpoint_length, endpoint_length, 3);
	}
	
	std::cout << sizeof(*buffer) << std::endl;
	return buffer;
}

#define MP_FUSE_USER            0x00
#define MP_FUSE_CFG             0x01
#define MP_FUSE_LOCK            0x02

Fuses avr_fuses[] =
{
{.name = "fuses", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 0 },
{.name = "lock_byte", .minipro_cmd = MP_FUSE_LOCK, .length = 1, .offset = 0 },
{.name = NULL }, };

Fuses avr2_fuses[] =
{
{.name = "fuses_lo", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 0 },
{.name = "fuses_hi", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 1 },
{.name = "lock_byte", .minipro_cmd = MP_FUSE_LOCK, .length = 1, .offset = 0 },
{.name = NULL }, };

Fuses avr3_fuses[] =
{
{.name = "fuses_lo", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 0 },
{.name = "fuses_hi", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 1 },
{.name = "fuses_ext", .minipro_cmd = MP_FUSE_CFG, .length = 1, .offset = 2 },
{.name = "lock_byte", .minipro_cmd = MP_FUSE_LOCK, .length = 1, .offset = 0 },
{.name = NULL }, };

Fuses pic_fuses[] =
{
{.name = "user_id0", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 0 },
{.name = "user_id1", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 2 },
{.name = "user_id2", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 4 },
{.name = "user_id3", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 6 },
{.name = "conf_word", .minipro_cmd = MP_FUSE_CFG, .length = 2, .offset = 0 },
{.name = NULL }, };

Fuses pic2_fuses[] =
{
{.name = "user_id0", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 0 },
{.name = "user_id1", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 2 },
{.name = "user_id2", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 4 },
{.name = "user_id3", .minipro_cmd = MP_FUSE_USER, .length = 2, .offset = 6 },
{.name = "conf_word", .minipro_cmd = MP_FUSE_CFG, .length = 2, .offset = 0 },
{.name = "conf_word1", .minipro_cmd = MP_FUSE_CFG, .length = 2, .offset = 2 },
{.name = NULL }, };