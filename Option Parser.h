#pragma once
#include "Commands.h"



namespace OptionParser
{
	bool getBoolOption(const ParsedCommand& cmd,
		std::string_view name,
		bool defaultValue);

	bool getBoolOption(std::string_view name);





	double getDoubleOption(const ParsedCommand& cmd,
		std::string_view name,
		double defaultValue);


	int getIntOption(const ParsedCommand& cmd,
		std::string_view name,
		int defaultValue);


	size_t getSizeOption(const ParsedCommand& cmd,
		std::string_view name,
		size_t defaultValue);




	
	std::string getLowerCaseStringOption(const ParsedCommand& cmd,
		std::string_view name,
		std::string defaultValue);

	std::string getLowerCaseStringOption(const ParsedCommand& cmd,
		std::string_view name);


	std::string getOriginalStringOption(const ParsedCommand& cmd,
		std::string_view name,
		std::string defaultValue);

}
