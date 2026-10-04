#include <string>
#include <iostream>
#include <windows.h>

#include "AMLE Loop.h"
#include "CommandParser.h"
#include "Command Executor.h"
#include "Persistence.h"
#include "FileSystem.h"





namespace
{
    void startUp()
    {

        // UTF-8 console support for UI/output.
        // AMLE text processing is currently ASCII-oriented.
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);


		std::cout<<R"(  
         █████╗  ███╗   ███╗ ██╗     ███████╗
        ██╔══██╗ ████╗ ████║ ██║     ██╔════╝
        ███████║ ██╔████╔██║ ██║     █████╗
        ██╔══██║ ██║╚██╔╝██║ ██║     ██╔══╝
        ██║  ██║ ██║ ╚═╝ ██║ ███████╗███████╗
        ╚═╝  ╚═╝ ╚═╝     ╚═╝ ╚══════╝╚══════╝
        )";

        UI::spacer();
      
        UI::Common::Header::printBigHeader("Welcome to AMLE - Adaptive Markov Language Engine");
        UI::Common::Header::printSmallHeader("Type '@help' for a list of available commands");
    }

}
namespace AMLE
{
    void Run(AIContext& Amle, CommandParser& Parser)
	{
        //palceholder for AutoSave when u decide to make it better
        Command lastCommand = Command::None;
        Target lastTarget = Target::None;

        startUp();

        while (true)
        {
            std::string input;

            std::getline(std::cin, input);

            //make it case sensitive just to make sure the user doesnt somehow by some miracle type that by accident
            if (input == "stop") break;

            ParsedCommand CMD;

            try
            {
                CMD = Parser.parse(input);
                Execute::Executor(CMD, Amle);

                lastCommand = CMD.command;
                lastTarget = CMD.target;


            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << "\n";
            }


        }



        if (!(lastCommand == Command::Model && lastTarget == Target::Save))
        {
            UI::Warning::printWarningText("You may have forgotten to save the model\n");


            std::cout << "Want to auto-save? y/n \n";
            std::string choice;
            std::getline(std::cin, choice);



            if (choice == "y" || choice == "Y")
            {
                std::string filename = "AUTO-SAVE-" + Persistence::Files::getDefaultName();

                Persistence::save(Amle, filename);
            }

            else 
            {
                std::cout << "Will take that as a no...\n";
            }
            


        }




	}


}
