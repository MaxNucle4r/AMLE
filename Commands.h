#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

#include "AIContext.h"






enum class Command
{
    //special commands
    Help,
    Clear,
    Version,


    Generate,
    Print,
    Train,
    Model,
    Config,
    Debug,

    None
};


static const std::unordered_map<std::string_view, Command> commands =
{
    //dont forget u havnt wrote anything for @help
    {"help", Command::Help},

    {"generate", Command::Generate},
    {"print", Command::Print},
    {"train", Command::Train},
    {"model", Command::Model},
    {"config", Command::Config},

    // for @[command]{ [options] } commands types
    {"debug", Command::Debug }
};


static const std::unordered_map<std::string_view, Command> specialCommands =
{
    {"help", Command::Help},
    {"cls", Command::Clear},
    {"version", Command::Version}
};





enum class Target
{
    Text,
    Word,
    Memory,
    Vocab,
    Transitions,
    Probs,
    Save,
    Load,
    Files,
    Input,

    //self train for alter
    Self,

    //for @config
    Generation,
    UI,
    Runtime,

    //for @debug
    DebugFlags,


    None
};


static const std::unordered_map<std::string_view, Target> targets =
{
    {"text", Target::Text},
    {"word", Target::Word},
    {"memory", Target::Memory},
    {"vocab", Target::Vocab},
    {"transitions", Target::Transitions},
    {"probs", Target::Probs},
    {"save", Target::Save},
    {"load", Target::Load},
    {"files", Target::Files,},
    {"input", Target::Input},
    {"flags", Target::DebugFlags},
    {"generation", Target::Generation},
    {"runtime", Target::Runtime},
    {"ui", Target::UI},
    {"self", Target::Self}

};





struct ParsedCommand
{
    Command command = Command::None;
    Target target = Target::None;

    std::unordered_map<std::string, std::string> options;

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






