#include <iostream>

#include "Commands.h"





std::string_view commandToString(Command command)
{
    switch (command)
    {
        case Command::Help:     return "help";
        case Command::Cls:      return "cls";
        case Command::Version:  return "version";
        case Command::Generate: return "generate";
        case Command::Print:    return "print";
        case Command::Train:    return "train";
        case Command::Model:    return "model";
        case Command::Config:   return "config";
        case Command::Debug:    return "debug";
        case Command::Clear:    return "clear";
        case Command::None:     return "none";
    }

    return "unknown";
}



std::string_view targetToString(Target target)
{
    switch (target)
    {
        case Target::Text:        return "text";
        case Target::Word:        return "word";
        case Target::Memory:      return "memory";
        case Target::Vocab:       return "vocab";
        case Target::Transitions: return "transitions";
        case Target::Probs:       return "probs";
        case Target::Save:        return "save";
        case Target::Load:        return "load";
        case Target::Files:       return "files";
        case Target::Input:       return "input";
        case Target::Self:        return "self";
        case Target::Generation:  return "generation";
        case Target::UI:          return "ui";
        case Target::Runtime:     return "runtime";
        case Target::DebugFlags:  return "flags";
        case Target::None:        return "none";
    }

    return "unknown";
}