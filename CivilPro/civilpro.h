#pragma once

#include <vector>

#include "device.h"
#include "tl_programmer.h"

enum class EnumArgCommands
{
	NONE,
	HELP,
	VERBOSE,
	INFO,
};

struct ArgCommand
{
	EnumArgCommands commandType;
	bool requiresValue = false;
	bool exclusiveFlag = false; // ONLY this flag can be passed, will throw error if more than 1 exclusive flag(s) are passed
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
	void PrintSystemInfo();

	TLProgrammer m_Programmer;
	std::vector<ArgCommand> m_Commands;
};