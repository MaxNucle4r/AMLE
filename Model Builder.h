#pragma once
#include <string>

#include "Types.h"





namespace Model
{

	namespace Text
	{
		// legacy model
		void buildTextTransitions(TextTypes::TransitionMap& transitions, 
			const std::vector<std::string>& tokens);
	
		// generic NGram
		void buildTextTransitionsNGram(TextTypes::NgramTransitionMap& transitions, const size_t contextSize,
			const std::vector<std::string>& tokens);
	
	}
	


	namespace Word
	{
		// legacy model
		void buildCharTransitions(CharTypes::TransitionMap& transitions, 
			const std::vector<std::string>& tokens);
		
		// generic NGram
		void buildCharTransitionsNGram(CharTypes::NgramTransitionMap& transitions, const size_t contextSize,
			const std::vector<std::string>& tokens);
	
	}
	

}



