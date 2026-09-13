#include <algorithm>
#include <sstream>

#include "Text Processing.h"
#include "AMLE_ASSERT.h"





//std::isalpha(c);   // A-Z, a-z
//std::isdigit(c);   // 0-9
//std::isalnum(c);   // letter or digit
//std::islower(c);   // a-z
//std::isupper(c);   // A-Z
//std::isspace(c);   // ' ', '\n', '\t', ...
//std::ispunct(c);   // .,!?:;"()[]{}...
//std::isprint(c);   // printable character
//std::iscntrl(c);   // control character
//std::isxdigit(c);  // hexadecimal digit





namespace TextProcessing
{
	bool Validate(std::string_view text, TextProcess& process)
	{

		if (text.empty())
		{
			process = TextProcess::EMPTY;
			return false;
		}



		bool allSpace = std::all_of(text.begin(), text.end(),
			[](unsigned char c) {return std::isspace(c); });


		if (allSpace)
		{
			process = TextProcess::ONLY_SPACES;
			return false;
		}



		bool allNumbers = std::all_of(text.begin(), text.end(),
			[](unsigned char c) {return std::isdigit(c); });


		if (allNumbers)
		{
			process = TextProcess::ONLY_NUMBERS;
			return false;
		}



		bool allNonAlpha = std::all_of(text.begin(), text.end(),
			[](unsigned char c) { return std::ispunct(c); });


		if (allNonAlpha)
		{
			process = TextProcess::ONLY_SYMBOLS;
			return false;
		}



		process = TextProcess::PASSED;
		return true;



	}
}




//THIS IS PURE MENTAL WAR WITH A WHOLE ASS LANGUAGE
static void punctuationNormalizer(WordData& word)
{
	if (word.wasCaps)
	{
		//case "?!"
		if (word.punctuation.size() >= 2 &&
			(word.punctuation[0] == '?' || word.punctuation[0] == '!') &&
			(word.punctuation[1] == '?' || word.punctuation[1] == '!') &&
			(word.punctuation[0] != word.punctuation[1])
			)
		{
			if (word.punctuation.size() >= 4)
				word.punctuation = word.punctuation.substr(0, 4);
		}


		//the other cases
		else if (word.punctuation.size() >= 3)
			word.punctuation = word.punctuation.substr(0, 3);

	}


	else
	{

		//maybe allow "hello!!" ? maaaaaaybe...
		//maybe not...
		//for now it becomes "hello!"
		//but check for "hello?!" tho...
		if (word.punctuation.size() > 1)
		{
			if (word.punctuation.size() > 2 &&
				(word.punctuation[0] == '?' || word.punctuation[0] == '!') &&
				(word.punctuation[1] == '?' || word.punctuation[1] == '!') &&
				(word.punctuation[0] != word.punctuation[1])
				)
			{
				word.punctuation = word.punctuation.substr(0, 2);
			}


			// "..." case assuming there isnt random shit after it
			//maybe modify the checker
			else if (word.punctuation[0] == '.')
			{
				word.punctuation = word.punctuation.substr(0, 3);
			}


			//whatever other cases
			else
			{
				word.punctuation = word.punctuation.substr(0, 1);
			}


		}



	}
}


static void letterRepetitionFixer(int maxRepetitions, int repetitions, std::string& word)
{
	for (size_t i = 1; i < word.size(); ++i)
	{
		if (word[i - 1] == word[i])
			repetitions++;

		else
			repetitions = 0;



		if (repetitions >= maxRepetitions)
		{
			size_t streakStart = i - repetitions;

			// first character we DONT want anymore
			size_t eraseBegin = streakStart + maxRepetitions;


			// find first different character
			size_t eraseEnd = eraseBegin;

			while (eraseEnd < word.size() &&
				word[eraseEnd] == word[streakStart])
			{
				++eraseEnd;
			}
			
			
			word.erase(eraseBegin, eraseEnd - eraseBegin);

			repetitions = maxRepetitions - 1;

			i = eraseBegin;

		}


	}
}

static void letterNormalizer(WordData& word)
{
	// "HELLOOO" becomes "hellooo"
	// "hellooo" becomes "hello"
	// what about "HEEEEEEELLO"... fcking hell


	std::transform(word.letters.begin(), word.letters.end(), word.letters.begin(),
		[](unsigned char c)
		{
			return static_cast<char>(std::tolower(c));
		});


	

	if (word.wasCaps)
	{
		int maxRepetitions = 5;
		int repetitions = 0;

		letterRepetitionFixer(maxRepetitions, repetitions, word.letters);

		
	}

	else
	{
		int maxRepetitions = 3;
		int repetitions = 0;

		letterRepetitionFixer(maxRepetitions, repetitions, word.letters);

	}



}



//These must all work. eventually.
//twenty-four
//can't
//well...
//hello!!!
//"hello"
//--test
//word—word

namespace
{

	enum class CharType
	{
		Letter,
		Punctuation,
		Numbers
			
	};

	CharType checkCharacter(unsigned char c)
	{
		if (std::isalpha(c))
			return CharType::Letter;

		if (std::ispunct(c))
			return CharType::Punctuation;

		if (std::isdigit(c))
			return CharType::Numbers;


		AMLE_ASSERT(false);
		std::unreachable();


	}



	bool capsChecker(std::string_view word)
	{
		return std::all_of(word.begin(), word.end(),
			[](unsigned char c) 
			{
				//to prevent cases like: HELLO123 to return false
				return !std::isalpha(c) || std::isupper(c);
			});
	}


}


namespace TextProcessing
{
	std::vector<WordData> Analyze(std::string_view text)
	{
		std::vector<WordData> analized;


		std::stringstream ss{ std::string(text) };
		std::string word;

		while (ss>>word)
		{

			//struct WordData
			//{
			//	std::string letters;
			//	std::string punctuation;

			//	bool wasCaps = false;
			//};

			
			auto start = word.begin();

			bool previousChunkCaps = false;

			while(start != word.end())
			{ 
				
				 
				WordData data;

				CharType type = checkCharacter(*start);


				auto current = start;

				while (current != word.end() && checkCharacter(*current) == type)
				{
					++current;
				}


				std::string_view chunk(start, current);



				if (type != CharType::Punctuation)
				{
					data.letters = std::string(chunk);
					data.wasCaps = capsChecker(data.letters);

					if (type == CharType::Letter)
						previousChunkCaps = data.wasCaps;

					letterNormalizer(data);

				}
				
				else
				{
					data.punctuation = std::string(chunk);

					data.wasCaps = previousChunkCaps;

					punctuationNormalizer(data);
				}
				

				analized.push_back(data);

				start = current;

			}


		}

		return analized;

	}

}





namespace TextProcessing
{

	std::string RebuildText(const std::vector<WordData>& words)
	{
		std::string final;

		for (const auto& word : words)
		{
			final += word.letters;

			if (!word.punctuation.empty())
			{
				final += " ";
				final += word.punctuation;
			}

			final += " ";
		}

		return final;

	}



	std::vector<std::string> Tokenize(const std::vector<WordData>& words)
	{
		std::vector<std::string> tokens;

		for (const auto& w : words)
		{
			if (!w.letters.empty())
				tokens.push_back(w.letters);

			if (!w.punctuation.empty())
				tokens.push_back(w.punctuation);

		} 


		return tokens;

	}


}

