#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

#include "AIContext.h"
#include "Commands.h"





struct ParsedCommand
{
    Command command = Command::None;
    Target target = Target::None;

    std::unordered_map<Option, std::string> options;

};



// COMMANDS WILL LOOK LIKE THIS
// @command>target{option/ option...}
//hopefully the syntax aint gonna change no more...
class CommandParser
{
private:

    Command parseCommand(std::string_view commandText);

    Target parseTarget(std::string_view targetText);

    void parseOptions(std::string_view optionsText,
                        ParsedCommand& parsed);



    Command checkSpecialCommands(std::string_view commandText);

    void checkDebugCommand(std::string_view input, ParsedCommand& CMD);


public:
    ParsedCommand parse(std::string_view input);

};






