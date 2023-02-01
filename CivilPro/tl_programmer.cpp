#include <iostream>
#include <string>

#include "tl_programmer.h"

static DeviceInfo devices[] =
{
#include "infoic2plus_devices.h"
	{.name = NULL},
};

ProgrammerInfo TLProgrammer::GetProgrammerInfo()
{
	if (!m_Programmer.IsDevicePresent())
	{
		std::cerr << "No programmer found!" << std::endl;
		exit(0);
	}

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

void TLProgrammer::BeginTransaction()
{

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