#include <string>
#include <iostream>
#include <algorithm>
#include <optional>



#include "CommandParser.h"
#include "UI.h"
#include "Generation.h"
#include "Debug.h"
#include "Score.h"





void CommandParser::checkDebugCommand(std::string_view input, ParsedCommand& CMD)
{
	auto optionStart = input.find('{');
	auto optionEnd = input.find('}', optionStart);



	if (optionEnd == std::string_view::npos ||
		optionStart == std::string_view::npos ||
		optionEnd < optionStart)
		throw std::invalid_argument("Malformed options");



	std::string_view commandText = input.substr(0, optionStart);



	// Only handle the shorthand @debug{...} syntax
	Command cmd = parseCommand(commandText);


	if (cmd != Command::Debug)
		return;

	CMD.command = cmd;
	CMD.target = Target::DebugFlags;



	std::string_view options = 
		input.substr(optionStart + 1, optionEnd - optionStart - 1);

	
	parseOptions(options, CMD);


}



Command CommandParser::checkSpecialCommands(std::string_view commandText)
{

	auto it = specialCommands.find(commandText);

	if (it != specialCommands.end())
		return it->second;

	return Command::None;


}


Command CommandParser::parseCommand(std::string_view commandText)
{
	auto it = commands.find(commandText);

	if (it != commands.end())
		return it->second;

	return Command::None;
}


Target CommandParser::parseTarget(std::string_view targetText)
{
	auto it = targets.find(targetText);

	if (it != targets.end())
		return it->second;

	return Target::None;
}


void CommandParser::parseOptions(std::string_view optionsText,
									ParsedCommand& parsed)
{
	while (!optionsText.empty())
	{

		auto slash = optionsText.find('/');


		std::string_view token;

		if (slash == std::string::npos)
		{
			token = optionsText;

			//dont forget string_view cant take ""
			//otherwise u will run into bugs... AGAIN.
			optionsText = {};
		}
		else
		{
			token = optionsText.substr(0, slash);
			optionsText = optionsText.substr(slash + 1);
		}
		 
		

		auto eq = token.find('=');
		if (eq == std::string_view::npos)
			throw std::invalid_argument("Invalid option format");


		std::string_view option = token.substr(0, eq);
		std::string_view value = token.substr(eq + 1);



		//careful what u use
		//this can overwrite things
		parsed.options[std::string(option)] = std::string(value);
		
		//this does not
		//parsed.options.emplace(option, value);


	}


}





ParsedCommand CommandParser::parse(std::string_view input)
{
	ParsedCommand CMD;


	// @command>target{option/option...}


	if (input.empty()) 
		throw std::invalid_argument("EMPTY COMMAND");

	if (input[0] != '@') 
		throw std::invalid_argument("Missing '@'");



	std::string_view s = input.substr(1);



	auto pos = s.find('>');
	if (pos == std::string_view::npos)
	{

		CMD.command = checkSpecialCommands(s);
		if (CMD.command != Command::None)
			return CMD;


		// @debug{...} is shorthand for @debug>flags{...}.
		// checkDebugCommand() sets both CMD.command and CMD.target.
		checkDebugCommand(s, CMD);
		if (CMD.command == Command::Debug)
			return CMD;


		throw std::invalid_argument("Missing '>'");
	}
		

	

	std::string_view commandText = s.substr(0, pos);

	std::string_view extra = s.substr(pos+1);


	Command command = parseCommand(commandText);
	if (command == Command::None)
		throw std::logic_error("Invalid command");

	CMD.command = command;



	auto optionStart = extra.find('{');

	std::string_view targetText;

	if (optionStart == std::string::npos)
	{
		targetText = extra;
	}

	else
	{
		targetText = extra.substr(0, optionStart);
	}


	Target target = parseTarget(targetText);

	if (target == Target::None)
		throw std::logic_error("Invalid Target");

	CMD.target = target;



	if (optionStart != std::string_view::npos)
	{
		auto optionEnd = extra.find('}');

		if (optionEnd == std::string_view::npos || optionEnd < optionStart)
			throw std::invalid_argument("Malformed options");
	
		
		std::string_view options =
			extra.substr(optionStart + 1, optionEnd - optionStart - 1);

		parseOptions(options, CMD);
	
	
	}

	
	return CMD;

}








