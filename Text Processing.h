#pragma once
#include <string>
#include <vector>

#include "Text Processing.h"





enum class TextProcess
{	NONE, 
	PASSED,
	EMPTY,
	ONLY_SPACES, 
	ONLY_NUMBERS,
	ONLY_SYMBOLS
};


struct WordData
{
	std::string letters;
	std::string punctuation;

	bool wasCaps = false;
};

namespace TextProcessing
{
	//this is proof i hate myself... and i like to make my life harder
	std::vector<WordData> Analyze(std::string_view text);

	std::string RebuildText(const std::vector<WordData>& words);

	std::vector<std::string> Tokenize(const std::vector<WordData>& words);

	bool Validate(std::string_view text, TextProcess& process);


}