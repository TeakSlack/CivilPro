#include "civilpro.h"

#include <iostream>

CivilPro::CivilPro(int argc, char* argv[])
{
	ProcessArgs(argc, argv);
	ExecuteArgs();
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

		if (std::strcmp(argv[i], "-v") == 0) command = { EnumArgCommands::VERBOSE };
		if (std::strcmp(argv[i], "-h") == 0) command = { EnumArgCommands::HELP, false, true };
		if (std::strcmp(argv[i], "-i") == 0) command = { EnumArgCommands::INFO, false, true };

		if (command.commandType != EnumArgCommands::NONE) m_Commands.push_back(command);
	}
}

void CivilPro::ExecuteArgs()
{
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
			EnableVerboseMode();
			break;
		case EnumArgCommands::HELP:
			PrintHelpText();
			break;
		case EnumArgCommands::INFO:
			PrintSystemInfo();
			break;
		}
	}
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

	std::cout << "Programmer Info: " << std::endl << std::endl;
	std::cout << "Device version: " << (int)info.device_version << std::endl;
	std::cout << "Hardware version: " << (int)info.hardware_version << std::endl;
	std::cout << "Firmware version: " << (int)info.firmware_version_major << (int)info.firmware_version_minor << std::endl;
}