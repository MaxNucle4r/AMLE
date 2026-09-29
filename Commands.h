#pragma once
#include <unordered_map>
#include <string_view>





enum class Command
{
    //special commands
    Help,
    Cls,
    Version,


    Generate,
    Print,
    Train,
    Model,
    Config,
    Debug,
    Clear,

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
    {"clear", Command::Clear},

    // for @[command]{ [options] } commands types
    {"debug", Command::Debug }
};


static const std::unordered_map<std::string_view, Command> specialCommands =
{
    {"help", Command::Help},
    {"cls", Command::Cls},
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

    //self train for later
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






