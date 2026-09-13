#pragma once
#include "Types.h"





namespace Model
{
	namespace Text
	{
		TextTypes::ProbabilityMap makeProbabilities(const TextTypes::TransitionMap& transitions);
	
		TextTypes::NgramProbabilityMap makeProbabilitiesNGram(const TextTypes::NgramTransitionMap& transitions);

	}



	namespace Char
	{
		CharTypes::ProbabilityMap makeProbabilities(const CharTypes::TransitionMap& charTransitions);
	
		CharTypes::NgramProbabilityMap makeProbabilitiesNGram(const CharTypes::NgramTransitionMap& transitions);

	}
}




