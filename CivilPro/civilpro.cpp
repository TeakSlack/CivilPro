#include "civilpro.h"

#include <iostream>

CivilPro::CivilPro(int argc, char* argv[])
{
	ProcessArgs(argc, argv);
	ExecuteArgs();
}

void CivilPro::ProcessArgs(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		ArgCommand command = { EnumArgCommands::NONE, false };

		if (std::strcmp(argv[i], "-v") == 0) command = {EnumArgCommands::VERBOSE, false};
		if (std::strcmp(argv[i], "-h") == 0) command = {EnumArgCommands::HELP, false};

		if (command.commandType != EnumArgCommands::NONE) m_Commands.push_back(command);
	}
}

void CivilPro::ExecuteArgs()
{
	for (ArgCommand command : m_Commands)
	{
		switch (command.commandType)
		{
		case EnumArgCommands::VERBOSE:
			std::cout << "verbose has been input" << std::endl;
			break;
		case EnumArgCommands::HELP:
			std::cout << "help has been input" << std::endl;
			break;
		}
	}
}