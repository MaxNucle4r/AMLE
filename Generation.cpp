#include "Generation.h"
#include "UI.h"
#include "RNG.h"

#include <iostream>
#include <sstream>
#include <algorithm>





namespace Seed
{

	void applySeed(SeedContext& seedctx, const std::optional<std::vector<std::string>>& seed)
	{
		if(!seed || seed->empty()) return;

		const auto& words = *seed;

		if (seed->size() == 1)
		{
			//TO AVOID ***[seed word] + <START>*** pair
			//completely forgot about this case till a bug showed up

			seedctx = { "<START>", words.back() };
			return;
			
		}
		
		seedctx = { words[words.size() - 2], words.back() };

	}



	std::vector<std::string> extractSeed(const std::string& seedString)
	{
		std::vector<std::string> seed;

		std::stringstream ss(seedString);
		std::string word;

		while (ss >> word)
		{
			std::transform(word.begin(), word.end(), word.begin(),
				[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

			seed.push_back(word);

		}


		return seed;


	}


	
}





namespace
{
	void appendString(std::string& string, const std::string& toAdd)
	{
		// hopefully this would turn "hello" + "!!!" + "how are u"
		// into "hello!!! how are u"

		if (std::all_of(toAdd.begin(), toAdd.end(),
			[](unsigned char c)
			{
				return std::ispunct(c);
			}))
		{
			string += toAdd;
		}

		else
		{
			string += " " + toAdd;
		}

	}



	void slideContext(TextTypes::Ngram& context, const std::string chosen)
	{
		context.erase(context.begin());
		context.push_back(chosen);
	}


}





namespace Generation::Detail
{
	template<typename Token>
	Token chooseToken(const std::unordered_map<Token, double>& nextMap,
		double temperature)
	{


		double total = 0.0;

		for (const auto& [context, prob] : nextMap)
			total += std::pow(prob, 1.0 / temperature);



		double randToken = distr(gen);

		double cumulative = 0.0;


		// safety fallback
		// in case of wierd shit happens with floating-point
		//return an existing token rather than leaving result invalid
		//nextMap is guaranteed to be non-empty 
		Token chosen = nextMap.begin()->first;


		for (const auto& [t, prob] : nextMap)
		{
			double adjusted = std::pow(prob, 1.0 / temperature);

			adjusted /= total;


			cumulative += adjusted;

			if (randToken <= cumulative)
			{
				chosen = t;

				break;
			}


		}


		return chosen;

	}

}





//legacy model
namespace Generation
{
	std::string Text(const TextTypes::ProbabilityMap& probs,
		double temperature,
		EndingType& ending, GenerationType& type,
		const std::optional<std::vector<std::string>>& seed,
		const Debug& debugger)
		//if no seed provided assume it is empty
	{

		std::string reply;		


		//DONT FORGET IT DEFAULTS TO <START> <START>
		//coz u gonna forget... 
		Seed::SeedContext seedcontext;

		if (seed  && !seed->empty())
		{
			Seed::applySeed(seedcontext, seed);

			if(debugger.enabled(DebugFlags::Seed))
			{
				std::cout << "\n *STARTING POINT: ";

				std::cout << (seedcontext.w1 != "<START>" ? seedcontext.w1 : " <NONE> ");
				std::cout << " --- ";
				std::cout << (seedcontext.w2 != "<START>" ? seedcontext.w2 : " <NONE> ");
				std::cout << "* \n";
			}

		}

		std::string w1 = seedcontext.w1;
		std::string w2 = seedcontext.w2;


		if (w1 != "<START>")
			reply += w1 + " ";

		if (w2 != "<START>")
			reply += w2 + " ";



		UI::Warning::temperature(temperature);

		while (true)
		{
			auto it = probs.find({ w1,w2 });

			//TURN THIS LATER INTO A BIGRAM FALLBACK
			//which hopefully wont need to turn into a unigram
			//but maybe just keep legacy pure.
			if (it == probs.end())
			{
				UI::Warning::GenerationFail::printContext(std::make_pair(w1, w2));

				ending = EndingType::UNFOUND_WORDS;

				break;
			}


			
			std::string chosen = Generation::Detail::chooseToken(it->second, temperature);

			if (debugger.enabled(DebugFlags::Generation))
				std::cout << "CHOSEN WORD: " << chosen << "\n";



			if (chosen == "<END>")
			{
				ending = EndingType::END_TOKEN;
				break;
			}



			appendString(reply, chosen);

			w1 = w2;
			w2 = chosen;

		}


		type = GenerationType::Text;


		return reply;

	}





	std::string Word(const CharTypes::ProbabilityMap& probs,
		double temperature,
		EndingType& ending, GenerationType& type,
		const Debug& debugger)
	{

		std::string reply;

		//start = $
		//end = #
		char c1 = '$';
		char c2 = '$';


		UI::Warning::temperature(temperature);

		while (true)
		{
			auto it = probs.find({ c1,c2 });

			//TURN THIS LATER INTO A BIGRAM FALLBACK
			//which hopefully wont need to turn into a unigram
			//maybe keep legacy pure...
			if (it == probs.end())
			{
				UI::Warning::GenerationFail::printContext(std::make_pair(c1, c2));

				ending = EndingType::UNFOUND_CHARS;
				break;
			}


			char chosen = Generation::Detail::chooseToken(it->second, temperature);

			if (debugger.enabled(DebugFlags::Generation))
				std::cout << "CHOSEN CHARACTER: " << chosen << "\n";


			if (chosen == '#')
			{
				ending = EndingType::END_TOKEN;
				break;
			}


			reply += chosen;
			c1 = c2;
			c2 = chosen;


		}


		type = GenerationType::Word;


		return reply;


	}

}




namespace Seed
{
	TextTypes::Ngram prepareNGramSeed(const std::optional<std::vector<std::string>>& seed,
		size_t nGramSize)
	{
		TextTypes::Ngram result(
			nGramSize,
			"<START>"
		);

		if (!seed || seed->empty())
			return result;


		if (seed->size() >= nGramSize)
		{
			//replaces everything in result with elements from that range
			// interesting function...
			// general form
			// vector.assign(first, last);
			// range is [first, last)
			result.assign(
				seed->end() - nGramSize,
				seed->end()
			);

			return result;
		}

		// general form
		//  std::copy(sourceBegin, sourceEnd, destinationBegin);
		std::copy(
			seed->begin(),
			seed->end(),
			result.begin() + (nGramSize - seed->size())
		);


		return result;

	}




}





namespace Generation
{


	std::string Text(const TextTypes::NgramProbabilityMap& probs,
		double temperature,
		EndingType& ending, GenerationType& type,
		const std::optional<std::vector<std::string>>& seed,
		const Debug& debugger,
		const size_t NGramSize)
	{


		std::string reply;

		TextTypes::Ngram context;


		context = Seed::prepareNGramSeed(seed, NGramSize);


		UI::Warning::temperature(temperature);



		while (true)
		{
			auto it = probs.find(context);

			// TO DO
			// here will be the fallback
			// recursively call with NGram with size -1 ?

			if (it == probs.end())
			{
				UI::Warning::GenerationFail::printContext(context);

				ending = EndingType::UNFOUND_WORDS;
				break;
			}



			std::string chosen = Generation::Detail::chooseToken(it->second, temperature);
			if (debugger.enabled(DebugFlags::Generation))
				std::cout << "CHOSEN WORD: " << chosen << "\n";



			if (chosen == "<END>")
			{
				ending = EndingType::END_TOKEN;

				break;

			}


			appendString(reply, chosen);

			slideContext(context, chosen);


		}


		type = GenerationType::Text;


		return reply;

	}


}










namespace GenerationHelpersToString
{
	std::string EndingTypeToString(const EndingType& ending)
	{
		switch (ending)
		{
			case EndingType::NONE:
				return "< NONE > ";

			case EndingType::END_TOKEN:
				return "< END TOKEN >";

			case EndingType::UNFOUND_WORDS:
				return "< UNFOUND WORDS >";

			case EndingType::UNFOUND_CHARS:
				return "< UNFOUND CHARS >";

			default:
				return "< UNKNOWN >";

		}



	}

	

	std::string GenerationTypeToString(const GenerationType& genType)
	{
		switch (genType)
		{
			case GenerationType::Text:
				return "< TEXT >";

			case GenerationType::Word:
				return "< WORD >";

			case GenerationType::NONE:
				return "< NONE >";

			default:
				return "< UKNOWN> ";
		}


	}



}




