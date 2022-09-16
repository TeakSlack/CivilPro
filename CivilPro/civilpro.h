#pragma once

#include <vector>

#include "device.h"

enum class EnumArgCommands
{
	NONE,
	HELP,
	VERBOSE,
};

struct ArgCommand
{
	EnumArgCommands command;
	bool requiresValue;
};

// Central state managment and command processing
class CivilPro
{
public:
	CivilPro(int argc, char* argv[]);

	void ProcessArgs(int argc, char* argv[]);
	void ExecuteArgs();
private:
	const UsbDevice m_Device;
	std::vector<ArgCommand> m_Commands;
};

