#pragma once

#include <vector>
#include <deque>

#include "device.h"
#include "tl_programmer.h"

enum class EnumArgCommands
{
	NONE,
	HELP,
	VERBOSE,
	INFO,
};

struct ArgToken
{
	EnumArgCommands commandType;
	bool requiresValue = false;
	bool exclusiveFlag = false; // ONLY this flag can be passed, will throw error if more than 1 exclusive flag(s) are passed
	EnumArgCommands requiredCommandType = EnumArgCommands::NONE;
};

struct CommandExecutor
{
	EnumArgCommands command;
	std::string value = 0;
};

// High level I/O
class CivilPro
{
public:
	CivilPro(const int& argc, char* argv[]);
	
	void EnableVerboseMode();
	void PrintHelpText() const;
	void PrintSystemInfo();
private:
	std::vector<ArgToken> TokenizeArgs(const int argc, char* argv[]);
	std::deque<CommandExecutor> ReorderArgs(const std::vector<ArgToken>& tokens);

	TLProgrammer m_Programmer;
};