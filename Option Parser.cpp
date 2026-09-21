#include <algorithm>
#include <stdexcept>

#include "Option Parser.h"





//std::runtime_error
//std::logic_error
//std::invalid_argument
//std::out_of_range
//std::overflow_error
//std::underflow_error




namespace OptionParser
{
	bool getBoolOption(const ParsedCommand& cmd,
		std::string_view name,
		bool defaultValue)
	{
		auto it = cmd.options.find(std::string(name));

		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}


		std::string value = it->second;


		std::transform(
			value.begin(),
			value.end(),
			value.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			}
		);


		if (value == "true")
			return true;


		if (value == "false")
			return false;


		UI::Warning::invalidOption(name, value, defaultValue);
		return defaultValue;

	}



	bool getBoolOption(std::string_view name)
	{
		std::string value = std::string(name);


		std::transform(
			value.begin(),
			value.end(),
			value.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			}
		);



		if (value == "true")
			return true;

		if (value == "false")
			return false;

		throw std::invalid_argument("Expected 'true' or 'false' \n");
	}

	



	double getDoubleOption(const ParsedCommand& cmd,
		std::string_view name,
		double defaultValue)
	{
		auto it = cmd.options.find(std::string(name));

		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}



		try
		{
			return std::stod(it->second);
		}
		//std::stood can throw 2 exceptions
		// "..." catches all exceptions
		// so "..." will get both overflow or wrong type error
		// interesting... never knew this exists.
		catch (...)
		{
			UI::Warning::invalidOption(name, it->second, defaultValue);
			return defaultValue;
		}


	}

	

	int getIntOption(const ParsedCommand& cmd,
		std::string_view name,
		int defaultValue)
	{
		auto it = cmd.options.find(std::string(name));

		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}
			



		try
		{
			return std::stoi(it->second);
		}
		catch (...)
		{
			UI::Warning::invalidOption(name, it->second, defaultValue);
			return defaultValue;
		}


	}





	std::string getLowerCaseStringOption(const ParsedCommand& cmd,
		std::string_view name,
		std::string defaultValue)
	{
		auto it = cmd.options.find(std::string(name));

		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}
			



		std::string option = it->second;



		std::transform(
			option.begin(),
			option.end(),
			option.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			}
		);

		return option;

	}


	// assumes it always exists
	// this is always used after .contains() or .find()
	std::string getLowerCaseStringOption(const ParsedCommand& cmd,
		std::string_view name)
	{
		auto it = cmd.options.find(std::string(name));


		//for future cases
		AMLE_ASSERT(it != cmd.options.end());


		std::string option = it->second;


		std::transform(
			option.begin(),	
			option.end(),
			option.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			}
		);

		return option;

	}



	std::string getOriginalStringOption(const ParsedCommand& cmd,
		std::string_view name,
		std::string defaultValue)
	{
		auto it = cmd.options.find(std::string(name));

		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}


		return it->second;

	}





	size_t getSizeOption(const ParsedCommand& cmd,
		std::string_view name,
		size_t defaultValue)
	{

		auto it = cmd.options.find(std::string(name));


		if (it == cmd.options.end())
		{
			UI::Warning::missingOption(name, defaultValue);

			return defaultValue;
		}



		int value;

		try
		{
			value = std::stoi(it->second);
		}
		catch (...)
		{
			UI::Warning::invalidOption(name, it->second, defaultValue);
			return defaultValue;
		}


		if (value < 0)
		{
			UI::Warning::invalidOption(name, it->second, defaultValue);
			return defaultValue;
		}
			


		 return static_cast<size_t>(value);


	}


	size_t getSizeOption(const ParsedCommand& cmd,
		std::string_view name)
	{

		auto it = cmd.options.find(std::string(name));


		if (it == cmd.options.end())
		{
			throw std::runtime_error("Option not found");

		}



		int value;

		try
		{
			value = std::stoi(it->second);
		}
		catch (...)
		{
			throw std::invalid_argument("Invalid Option");
		}


		if (value < 0)
		{
			throw std::invalid_argument("Size must be greater than 0");
		}



		return static_cast<size_t>(value);


	}


} 

