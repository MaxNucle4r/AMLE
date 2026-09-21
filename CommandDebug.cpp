#include "CommandDebug.h"
#include "Option Parser.h"






static void debugModifier(AIContext& ctx, const ParsedCommand& cmd)
{

	if (cmd.options.contains("all"))
	{

		bool all = OptionParser::getBoolOption(cmd.options.at("all"));

		if (all)
			ctx.getDebugger().enableAll();
		else
			ctx.getDebugger().disableAll();
	}



	for (const auto& [name, value] : cmd.options)
	{
		auto it = debugMap.find(name);

		if (it == debugMap.end())
			continue;


		bool enabled = OptionParser::getBoolOption(value);


		if (enabled)
			ctx.getDebugger().enable(it->second);

		else
			ctx.getDebugger().disable(it->second);
	}


}

namespace Execute::Debugger
{
	void Apply(AIContext& ctx, const ParsedCommand& cmd)
	{
		if (cmd.options.empty())
			throw std::logic_error("At least 1 argument must exist\n");


		//in case u add more later
		//who knows
		switch (cmd.target)
		{

		case Target::DebugFlags:
			debugModifier(ctx, cmd);
			break;


		default:
			throw std::invalid_argument("Invalid target for @config>\n");
		}


	}
}



