#pragma once
#include "Types.h"
#include "Memory.h"
#include "CommandParser.h"
#include "Vocabulary.h"





void choosePrinter(const ParsedCommand& cmd,
	const AIContext& ctx);

namespace Execute::Print
{
	void TextTransitions(const TextTypes::TransitionMap& transitions);
	void TextProbabilities(const TextTypes::ProbabilityMap& probs);

	void NGramTextTransitions(const TextTypes::NgramTransitionMap& transitions);
	void NGramTextProbabilities(const TextTypes::NgramProbabilityMap& probs);



	void CharTransitions(const CharTypes::TransitionMap& transitions);
	void CharProbabilities(const CharTypes::ProbabilityMap& probs);


	void NGramCharTransitions(const CharTypes::NgramTransitionMap& transitions);
	void NGramCharProbabilities(const CharTypes::NgramProbabilityMap& probs);



	void Memory(const ParsedCommand& cmd, const AIMemory& memory);


	void Vocabulary(const VocabularyManager& vocab);

	//void ModelFiles();

}