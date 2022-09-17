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
	EnumArgCommands commandType;
	bool requiresValue = false;
	EnumArgCommands requiredCommandType = EnumArgCommands::NONE;
};

// Central state managment and command processing
class CivilPro
{
public:
	CivilPro(int argc, char* argv[]);

	void ProcessArgs(int argc, char* argv[]);
	void ExecuteArgs();
private:
	void EnableVerboseMode();
	void PrintHelpText();

	UsbDevice m_Device;
	std::vector<ArgCommand> m_Commands;
};