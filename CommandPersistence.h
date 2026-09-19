#pragma once
#include "AIContext.h"
#include "Commands.h" 





namespace Execute::Model
{
	void Save(const AIContext& ctx, const ParsedCommand& cmd);

	void Load(AIContext& ctx, const ParsedCommand& cmd);
}
