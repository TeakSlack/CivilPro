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
		ArgCommand command = { EnumArgCommands::NONE };

		if (std::strcmp(argv[i], "-v") == 0) command = { EnumArgCommands::VERBOSE };
		if (std::strcmp(argv[i], "-h") == 0) command = { EnumArgCommands::HELP };

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
			EnableVerboseMode();
			break;
		case EnumArgCommands::HELP:
			PrintHelpText();
			break;
		}
	}
}

void CivilPro::EnableVerboseMode()
{
	std::cout << "CivilPro has been launched in verbose mode." << std::endl;
	m_Device.SetVerboseWrite();
}

// TODO: print help blurb
void CivilPro::PrintHelpText()
{

}