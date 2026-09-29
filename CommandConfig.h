#pragma once
#include "AIContext.h"
#include "CommandParser.h"





namespace Execute::Config
{
	void Apply(AIContext& ctx, const ParsedCommand& cmd);

}