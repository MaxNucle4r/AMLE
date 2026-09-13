#pragma once
#include <string_view>
#include <iostream>





namespace UI::Warning
{
	template<typename Value>
	void invalidOption(std::string_view optionName, std::string_view optionReceived,
		const Value& defaultValue)
	{
		std::cout
			<< "Invalid value '" << optionReceived
			<< "' for option '" << optionName << "'.\n"
			<< "Using default: "
			<< std::boolalpha
			<< defaultValue
			<< std::noboolalpha
			<< '\n';


		std::cout << "\n\n\n";

	}





	template<typename Value>
	void missingOption(
		std::string_view optionName,
		const Value& defaultValue)
	{
		std::cout
			<< "Option '" << optionName << "' was not provided.\n"
			<< "Using default: "
			<< std::boolalpha
			<< defaultValue
			<< std::noboolalpha
			<< '\n';

		std::cout << "\n\n\n";
	}
}

