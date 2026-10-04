#include <iostream>

#include "CommandHelp.h"
#include "Commands.h"
#include "UI.h"
#include "Color Text.h"





namespace Execute::Help
{
	void showAllCommands()
	{
		for (const auto& [command, TargetMap] : commandRegistry)
		{

			UI::Common::Header::printBigHeader("Command: " + std::string(commandToString(command)));



			for (const auto& [target, OptionsMap] : TargetMap)
			{
				UI::Common::Header::printSmallHeader("Target: " + std::string(targetToString(target)));



				for (const auto& [option, definition] : OptionsMap)
				{
					{
					
						ScopedColor color(2);
						UI::Common::printText("Option: " + std::string(definition.name));

					}
					
					

					UI::Common::printText("Description: " + std::string(definition.description));

					UI::spacer();
				}


				UI::spacer();

			}


		}

	}


}