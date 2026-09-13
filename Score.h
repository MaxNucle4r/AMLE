#pragma once
#include <string>
#include <vector>

#include "Vocabulary.h"
#include  "Debug.h"





namespace Scoring
{
	namespace Word
	{
		float score(const std::string& word, 
					const VocabularyManager& vocab,
					const Debug& debugger);
	}


	namespace Text
	{
		float score(const std::string& text,
					const VocabularyManager& vocab,
					bool ignoreLenghtPenalty,
					const Debug& debugger);
	}

}



