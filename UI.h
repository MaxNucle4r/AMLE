#pragma once
#include "Types.h"
#include "Memory.h"





class VocabularyManager;

namespace UI
{
	void spacer();

	namespace Common
	{
		void printText(std::string_view text);

		void printEmpty(std::string_view emptyThing);


		namespace Header
		{
			void printSmallHeader(std::string_view smallHeader);

			void printBigHeader(std::string_view bigHeader);
		}


	}
	

	
	namespace Detail
	{
		template<typename Token>
		void printContext(const std::vector<Token>& context);
	}



	namespace Legacy
	{

		void printTransitions(const TextTypes::TransitionMap & transitions);

		void printProbabilities(const TextTypes::ProbabilityMap& probs);



		void printProbabilities(const CharTypes::ProbabilityMap& charProbs);

		void printTransitions(const CharTypes::TransitionMap& charTransitions);

		


	}



	namespace NGram
	{
		
		void printTransitions(const TextTypes::NgramTransitionMap& transitions);

		void printProbabilities(const TextTypes::NgramProbabilityMap& probs);



		void printTransitions(const CharTypes::NgramTransitionMap& charTransitions);

		void printProbabilities(const CharTypes::NgramProbabilityMap& charProbs);

	}
	


	namespace Memory
	{
		void printMemory(const std::vector<MemoryEntry>& entries);
	}



	namespace Warning
	{
		void temperature(const double& temperature);

		void maxWordCount(const int& wordCount);

		void printWarningText(std::string_view text);



		template<typename Value>
		void invalidOption(std::string_view optionName, std::string_view optionReceived, const Value& defaultValue);


		template<typename Value>
		void missingOption(std::string_view optionName, const Value& defaultValue);



		namespace GenerationFail
		{
			void printContext(const std::vector<std::string>& strings);

			void printContext(const std::pair<std::string, std::string>& strings);

			void printContext(const std::vector<char>& chars);

			void printContext(const std::pair<char, char>& chars);
		}
		


	}



	namespace Command
	{
		void showAllCommands();
	}



	namespace Vocab
	{
		void Vocabulary(const VocabularyManager& vm);
	}
	

}





#include "UITemplates.h"