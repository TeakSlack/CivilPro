#include "civilpro.h"

#include <iostream>
#include <string>
#include <algorithm>

CivilPro::CivilPro(const int& argc, char* argv[])
{
	std::vector<ArgToken> tokens = TokenizeArgs(argc, argv);
	std::deque<CommandExecutor> orderedCommands = ReorderArgs(tokens);
}

// Handles tokenization of parameters passed to CivilPro. It not handle execution of these tokens ("-d" being the only exception, not compiled when set to release)
std::vector<ArgToken> CivilPro::TokenizeArgs(const int argc, char* argv[])
{
	std::vector<ArgToken> commands;
	const char* exit_blurb = "Please rerun CivilPro with proper arguments or with -h for help.";

	if (argc == 1)
	{
		std::cout << exit_blurb << std::endl;
		exit(0);
	}

#ifdef _DEBUG
	// TODO: make this its own function.
	std::vector<std::string> args = { "civilpro" };

	if (std::strcmp(argv[1], "-d") == 0)
	{
		std::cout << "CivilPro launched in debug mode. Please input command line parameters: " << std::endl;

		std::string argstream;
		std::getline(std::cin, argstream);

		size_t arg_pos = argstream.find(" ");

		// seperate input into a vector of strings by space delimiter.
		do
		{
			args.push_back(argstream.substr(0, arg_pos));
			argstream.erase(0, arg_pos + 1);
		} while (arg_pos = argstream.find(" ") != std::string::npos);

		if (!argstream.empty()) args.push_back(argstream);
	}

// Very hacky, although it is the easiest solution and is analogous to normal code when set to release.
#define argc args.size()
#define arg args[i].c_str()
#else
#define arg argv[i]
#endif

	for (int i = 0; i < argc; i++)
	{
		ArgToken command = { EnumArgCommands::NONE };

		if (std::strcmp(arg, "-v") == 0) command = { EnumArgCommands::VERBOSE };
		else if (std::strcmp(arg, "-h") == 0) command = { EnumArgCommands::HELP, false, true };
		else if (std::strcmp(arg, "-i") == 0) command = { EnumArgCommands::INFO, false, true };

		if (command.commandType != EnumArgCommands::NONE) commands.push_back(command);
	}

	return commands;
}

std::deque<CommandExecutor> CivilPro::ReorderArgs(const std::vector<ArgToken>& tokens)
{
	// This is not the most efficient solution but it is the easiest to program.
	std::deque<CommandExecutor> commandExecutionOrder;

	int exclusiveFlags = 0;

	if (tokens.size() > 1)
	{
		auto loc = std::find_if(tokens.begin(), tokens.end(), [](const ArgToken& token) 
			{
				if (token.exclusiveFlag) return true;
				else return false;
			});
		
		if (loc != tokens.end())
		{
			std::cout << "Program ran with more than one exclusive flags!" << std::endl;
			exit(0);
		}
	}

	for (const ArgToken &command : tokens)
	{
		if (command.exclusiveFlag) exclusiveFlags++;
		if (exclusiveFlags >= 2)
		{
			std::cout << "Program ran with more than one exclusive flags!" << std::endl;
			exit(0);
		}

		switch (command.commandType)
		{
		case EnumArgCommands::VERBOSE:
			commandExecutionOrder.push_front({ EnumArgCommands::VERBOSE, "" });
			break;
		case EnumArgCommands::HELP:
			commandExecutionOrder.push_back({ EnumArgCommands::HELP, "" });
			break;
		case EnumArgCommands::INFO:
			commandExecutionOrder.push_back({ EnumArgCommands::INFO, "" });
			break;
		}
	}

	return commandExecutionOrder;
}

void CivilPro::EnableVerboseMode()
{
	std::cout << "CivilPro has been launched in verbose mode." << std::endl;
	m_Programmer.SetVerbose();
}

// TODO: print help blurb
void CivilPro::PrintHelpText() const
{
	std::cout << "do this" << std::endl;
}

void CivilPro::PrintSystemInfo()
{
	ProgrammerInfo info = m_Programmer.GetProgrammerInfo();

	const char* deviceName = (info.device_version == 5) ? "TL866II+" : "Unknown device!";

	std::cout << "Programmer Info: " << std::endl << std::endl;
	std::cout << "Device: " << deviceName << std::endl;
	std::cout << "Firmware version: " << (int)info.hardware_version << "." << (int)info.firmware_version_major << "." << (int)info.firmware_version_minor << std::endl;
}