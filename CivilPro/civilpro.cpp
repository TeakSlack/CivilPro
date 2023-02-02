#include "civilpro.h"

#include <iostream>

CivilPro::CivilPro()
{
	m_Device = m_TLProgrammer.GetDevice("sst39sf020a"); // get device from lookup table

	if (m_Device == nullptr) // checks if device is in table
	{
		std::cout << "Device not found!" << std::endl;
		exit(0);
	}

	Read();
}

void CivilPro::Read()
{
	m_TLProgrammer.BeginTransaction(m_Device);

	int blocks = m_Device->code_memory_size / m_Device->read_buffer_size; // gets number of 64 byte blocks to read

	if (m_Device->code_memory_size % m_Device->read_buffer_size != 0) // if last block is less than 64 bytes long, add one more block
		blocks += 1;

	char* buffer = new char[m_Device->code_memory_size];

	for (int i = 0; i < blocks; i++)
	{
		uint32_t address = i * m_Device->read_buffer_size;

		if (m_Device->opts4 & 0x2000) // TODO: figure out what this does
		{
			address = address >> 1;
		}

		uint32_t length = m_Device->read_buffer_size; // length to read

		if ((i + 1) * length > m_Device->code_memory_size) // gets size of last block to read
			length = m_Device->code_memory_size % length;

		char* block = m_TLProgrammer.ReadBlock(length, address);
		memset(buffer + i * m_Device->read_buffer_size, *block, length); // memory trickery to fill buffer at the proper location
	}
}