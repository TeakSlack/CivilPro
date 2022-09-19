#include "civilpro.h"

#include <iostream>
#include <string>
#include <deque>

CivilPro::CivilPro(int argc, char* argv[])
{
	ProcessArgs(argc, argv);
}

void CivilPro::ProcessArgs(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "Please rerun CivilPro with proper arguments or with -h flag for help." << std::endl;
		exit(0);
	}

	for (int i = 0; i < argc; i++)
	{
		ArgCommand command = { EnumArgCommands::NONE };

#ifdef _DEBUG
		if (std::strcmp(argv[i], "-d") == 0)
		{
			std::cout << "CivilPro launched in debug mode. Please input command line parameters: " << std::endl;

			std::string args;
			std::getline(std::cin, args);	
		}
#endif

		if (std::strcmp(argv[i], "-v") == 0) command = { EnumArgCommands::VERBOSE };
		if (std::strcmp(argv[i], "-h") == 0) command = { EnumArgCommands::HELP, false, true };
		if (std::strcmp(argv[i], "-i") == 0) command = { EnumArgCommands::INFO, false, true };

		if (command.commandType != EnumArgCommands::NONE) m_Commands.push_back(command);
	}
}

std::deque<CommandExectuor> CivilPro::ReorganizeArgs()
{
	std::deque<CommandExectuor> commandExecutionOrder;
	
	int exclusiveFlags = 0;

	for (ArgCommand command : m_Commands)
	{
		if (command.exclusiveFlag) exclusiveFlags++;
		if (exclusiveFlags > 1)
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
void CivilPro::PrintHelpText()
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