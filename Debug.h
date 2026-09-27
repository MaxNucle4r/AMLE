#pragma once
#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "Components.h"





enum class DebugFlags
{
    None = 0,

    Generation = 1 << 0,
    Seed = 1 << 1,
    Probabilities = 1 << 2,
    Memory = 1 << 3,
    Training = 1 << 4,
    Persistence = 1 << 5,
    Scoring = 1 << 6,

    All = Generation | Seed | Probabilities | Memory | Training | Persistence | Scoring
};

 
static const std::unordered_map<std::string_view, DebugFlags> debugMap =
{
    {"generation", DebugFlags::Generation},
    {"seed", DebugFlags::Seed},
    {"probabilities", DebugFlags::Probabilities},
    {"memory", DebugFlags::Memory},
    {"training", DebugFlags::Training},
    {"persist", DebugFlags::Persistence},
    {"scoring", DebugFlags::Scoring}
};



//make sure it is exactly 32 bits
// because C++ only guarantees minimum sizes for some reason
// and make it unsigned as well
using DebugMask = uint32_t;


class Debug
{

private:

    const ComponentID component = ComponentID::DEBUGGER;

    DebugMask mask = 0;



public:

    const ComponentID getID()
    {
        return component;
    }



    void enable(DebugFlags flag);

    void disable(DebugFlags flag);

    bool enabled(DebugFlags flag) const;

    void enableAll();

    void disableAll();




};



DebugFlags operator| (DebugFlags a, DebugFlags b);
DebugFlags operator&(DebugFlags a, DebugFlags b);
DebugFlags operator~(DebugFlags a);


