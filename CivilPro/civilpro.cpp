#include "civilpro.h"

#include <iostream>

CivilPro::CivilPro(int argc, char* argv[])
{
	if (!m_Device.IsDevicePresent())
	{
		std::cout << "Programmer not found! Please make sure the device is plugged in and the connector is secured." << std::endl;
		return;
	}

	ProcessArgs(argc, argv);
}

void CivilPro::ProcessArgs(int argc, char* argv[])
{

	for (int i = 0; i < argc; i++)
	{
		ArgCommand command = { EnumArgCommands::NONE, false };

		if (argv[i] == "-v" || argv[i] == "--verbose") command = {EnumArgCommands::VERBOSE, false};
		else if (argv[i] == "-h" || argv[i] == "--help") command = {EnumArgCommands::HELP, false};

		if (command.command != EnumArgCommands::NONE) m_Commands.push_back(command);
	}
}

