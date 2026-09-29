#pragma once
#include <string>
#include <optional>

#include "Types.h"
#include "Debug.h"
#include "CommandParser.h"





enum class EndingType { NONE, END_TOKEN, UNFOUND_WORDS, UNFOUND_CHARS };

enum class GenerationType { NONE, Word, Text };




namespace Seed
{
	struct SeedContext
	{
		std::string w1 = "<START>";
		std::string w2 = "<START>";
	};



	void applySeed(SeedContext& seedctx, const std::optional<SeedContext>& seed);

	std::vector<std::string> extractSeed(const std::string& text);



	TextTypes::Ngram prepareNGramSeed(const std::optional<std::vector<std::string>>& seed,
		size_t nGramSize);



}



namespace Generation
{
	

	namespace Detail
	{
		template<typename Token>
		Token chooseToken(
			const std::unordered_map<Token, double>& nextMap,
			double temperature);
	}
	


	std::string Text(const TextTypes::ProbabilityMap& probs, 
		double temperature, 
		EndingType& ending, GenerationType& type, 
		const std::optional<std::vector<std::string>>& seed,
		const Debug& debugger);


	std::string Text(const TextTypes::NgramProbabilityMap& probs,
		double temperature,
		EndingType& ending, GenerationType& type,
		const std::optional<std::vector<std::string>>& seed,
		const Debug& debugger,
		const size_t NGramSize);





	std::string Word(const CharTypes::ProbabilityMap& probs, 
		double temperature, 
		EndingType& ending, GenerationType& type,
		const Debug& debugger);


	std::string Word(const CharTypes::NgramProbabilityMap& probs,
		double temperature,
		EndingType& ending, GenerationType& type,
		const Debug& debugger);

}



namespace GenerationHelpersToString
{
	std::string EndingTypeToString(const EndingType& ending);

	std::string GenerationTypeToString(const GenerationType& gen);
}




