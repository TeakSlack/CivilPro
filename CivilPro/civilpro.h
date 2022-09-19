#pragma once

#include <vector>
#include <deque>

#include "device.h"
#include "tl_programmer.h"

enum class EnumArgCommands
{
#ifdef _DEBUG
	DEBUG,
#endif
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

struct CommandExectuor
{
	EnumArgCommands command;
	std::string value = 0;
};

// Central state managment and command processing
class CivilPro
{
public:
	CivilPro(int argc, char* argv[]);
	
	void EnableVerboseMode();
	void PrintHelpText();
	void PrintSystemInfo();
private:
	void ProcessArgs(int argc, char* argv[]);
	std::deque<CommandExectuor> ReorganizeArgs();

	TLProgrammer m_Programmer;
	std::vector<ArgCommand> m_Commands;
};