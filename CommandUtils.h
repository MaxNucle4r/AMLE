#pragma once
#include <string_view>

#include "Commands.h"





namespace CommandUtils
{
    namespace Commands
    {
        inline std::string_view commandToString(const Command& command)
        {
            switch (command)
            {
                case Command::Help: return "help";
                case Command::Cls:  return "cls";
                case Command::Version:  return "version";
                case Command::Generate: return "generate";
                case Command::Print: return "print";
                case Command::Train: return "train";
                case Command::Model: return "model";
                case Command::Config: return "config";
                case Command::Debug: return "debug";
                case Command::Clear: return "clear";

                default: return "none";
            }
        }
    }



    namespace Targets
    {
        inline std::string_view targetToString(const Target& target)
        {
            switch (target)
            {
                case Target::Text: return "text";
                case Target::Word: return "word";
                case Target::Memory: return "memory";
                case Target::Vocab: return "vocabulary";
                case Target::Transitions: return "transitions";
                case Target::Probs: return "probabilities";
                case Target::Save: return "save";
                case Target::Load: return "load";
                case Target::Files: return "files";
                case Target::Input: return "input";
                case Target::Self: return "model";
                case Target::Generation: return "generation";
                case Target::UI: return "UI";
                case Target::Runtime: return "runtime";
                case Target::DebugFlags: return "debug flags";

                default: return "unknown";
            }
        }
    }

    
}