#include <iostream>

#include "Memory.h"
#include "UI.h"
#include "Vocabulary.h"

#include "Color Text.h"






namespace UI
{

	void spacer()
	{
		std::cout << "\n\n\n";
	}

	namespace Common
	{
		void printText(std::string_view text)
		{
			std::cout << "\n" << text << "\n";

		}



		void printEmpty(std::string_view emptyThing)
		{
			ScopedColor color(8);

			std::cout << "*** (" << emptyThing << " is empty) *** ";

			spacer();
		}


		namespace Header
		{
			void printSmallHeader(std::string_view header)
			{
				ScopedColor color(15);

				std::cout << "===== " << header << " ===== \n";
				spacer();
			}


			void printBigHeader(std::string_view bigHeader)
			{
				ScopedColor color(15);

				const size_t width = bigHeader.size() * 2;

				std::cout << std::string(width, '=') << "\n";
				std::cout << std::string((width - bigHeader.size()) / 2, ' ') << bigHeader << "\n";
				std::cout << std::string(width, '=') << "\n";


				spacer();


			}

		}

		
	}
	



	namespace Detail
	{
		template<typename Token>
		void printContext(const std::vector<Token>& context)
		{
			std::cout << "( ";

			for (size_t i = 0; i < context.size(); ++i)
			{

				if (i + 1 == context.size())
				{
					std::cout << context[i];
				}

				else
				{
					std::cout << context[i] << ", ";
				}

			}

			std::cout << " )" << ": \n";


		}

	}

}



namespace UI::Legacy
{

	void printTransitions(const TextTypes::TransitionMap& transitions)
	{
		UI::Common::Header::printBigHeader("Legacy");
		UI::Common::Header::printSmallHeader("Text Transitions");

		if (transitions.empty())
		{
			UI::Common::printEmpty("Text Transitions");
			return;
		}



		for (const auto& [pair, nextWords] : transitions)
		{
			std::cout << "( " << pair.first << ", " << pair.second << " )" << ": \n";

			for (const auto [nextWord, count] : nextWords)
			{
				std::cout << "---> " << nextWord
					<< ": " << count << "\n";
			}

			std::cout << "\n";


		}


	}



	void printProbabilities(const TextTypes::ProbabilityMap& probs)
	{
		UI::Common::Header::printBigHeader("Legacy"); 
		UI::Common::Header::printSmallHeader("Text Probabilities");

		if (probs.empty()) 
		{
			UI::Common::printEmpty("Text Probabilities");
			return;
		}



		for (const auto& [pair, NextWords] : probs)
		{
			std::cout << "( " << pair.first << ", " << pair.second << " )" << ": \n";

			for (const auto& [nextWord, prob] : NextWords)
			{
				std::cout << "---> " << nextWord
					<< ": " << prob << "\n";
			}

			std::cout << "\n";


		}


	}





	void printTransitions(const CharTypes::TransitionMap& charTransitions)
	{
		UI::Common::Header::printBigHeader("Legacy");
		UI::Common::Header::printSmallHeader("Char Transitions");

		if (charTransitions.empty())
		{
			UI::Common::printEmpty("Char Transitions");
			return;

		}



		for (const auto& [pair, nextMap] : charTransitions)
		{
			std::cout << "( " << pair.first << ", " << pair.second << " )" << ": \n";

			for (const auto& [nextChar, count] : nextMap)
			{
				std::cout << "---> " << nextChar
					<< ": " << count << "\n";
			}

			std::cout << "\n";

		}


	}



	void printProbabilities(const CharTypes::ProbabilityMap& charProbs)
	{
		UI::Common::Header::printBigHeader("Legacy");
		UI::Common::Header::printSmallHeader("Char Probabilities");

		if (charProbs.empty())
		{
			UI::Common::printEmpty("Char Probabilities");
			return;
		}



		for (const auto& [pair, nextMap] : charProbs)
		{
			std::cout << "( " << pair.first << ", " << pair.second << " )" << ": \n";

			for (const auto& [nextChar, prob] : nextMap)
			{
				std::cout << "---> " << nextChar
					<< ": " << prob << "\n";
			}

			std::cout << "\n";


		}


	}



}





// intellisense is acting wierd
// seems like it hates UI::NGram
// so i will just put it in a separate namespace
namespace UI
{
	namespace NGram
	{
		void printTransitions(const TextTypes::NgramTransitionMap& transitions)
		{
			UI::Common::Header::printBigHeader("NGram");
			UI::Common::Header::printSmallHeader("Text Transitions");

			if (transitions.empty())
			{
				UI::Common::printEmpty("Text Transitions");
				return;
			}



			for (const auto& [context, nextMap] : transitions)
			{
				UI::Detail::printContext(context);


				for (const auto& [nextWord, count] : nextMap)
				{
					std::cout << "---> " << nextWord
						<< ": " << count << "\n";
				}


				std::cout << "\n";


			}



		}



		void printTransitions(const CharTypes::NgramTransitionMap& charTransitions)
		{
			UI::Common::Header::printBigHeader("NGram");
			UI::Common::Header::printSmallHeader("Char Transitions");


			if (charTransitions.empty())
			{
				UI::Common::printEmpty("Char Transitions");
				return;

			}



			for (const auto& [context, nextMap] : charTransitions)
			{
				UI::Detail::printContext(context);


				for (const auto& [nextChar, count] : nextMap)
				{
					std::cout << "---> " << nextChar
						<< ": " << count << "\n";
				}

				std::cout << "\n";

			}




		}



		void printProbabilities(const TextTypes::NgramProbabilityMap& probs)
		{
			UI::Common::Header::printBigHeader("NGram");
			UI::Common::Header::printSmallHeader("Text Probabilities");


			if (probs.empty())
			{
				UI::Common::printEmpty("Text Probabilities");
				return;
			}



			for (const auto& [context, nextMap] : probs)
			{
				UI::Detail::printContext(context);


				for (const auto& [nextWord, prob] : nextMap)
				{
					std::cout << "---> " << nextWord
						<< ": " << prob << "\n";
				}


				std::cout << "\n";

			}

		}



		void printProbabilities(const CharTypes::NgramProbabilityMap& charProbs)
		{
			UI::Common::Header::printBigHeader("NGram");
			UI::Common::Header::printSmallHeader("Char Probabilities");


			if (charProbs.empty())
			{
				UI::Common::printEmpty("Char Probabilities");
				return;
			}



			for (const auto& [context, nextMap] : charProbs)
			{
				UI::Detail::printContext(context);


				for (const auto& [nextChar, prob] : nextMap)
				{
					std::cout << "---> " << nextChar
						<< ": " << prob << "\n";
				}

				std::cout << "\n";

			}

		}
	
	}


}





namespace UI::Memory
{

	//TO DO
	//make the class have its own printer like Vocabulary maybe
	void printMemory(const std::vector<MemoryEntry>& entries)
	{
		UI::Common::Header::printSmallHeader("Memory");

		if (entries.empty())
		{
			UI::Common::printEmpty("Memory");
			return;
		}
			


		for (const auto& e : entries)
		{
			std::cout << "I: " << e.input << "\n";
			std::cout << "O: " << e.output << "\n\n";
		}
	}

}





namespace UI::Warning
{
	
	void printWarningText(std::string_view text)
	{
		ScopedColor color(6);

		spacer();
		std::cout << text << "\n";
		spacer();
	}



	//this... i think is useless since there are some other functions that check for this but throw
	// or use default values
	void temperature(const double& temperature)
	{
		spacer();

		if (temperature < 0.1)
		{
			std::cout << "TEMPERATURE TOO LOW\n";
		}
		else if (temperature > 2.5)
		{
			std::cout << "TEMPERATURE TOO HIGH\n";
		}


	}





	void maxWordCount(const int& wordCount)
	{
		spacer();

		if (wordCount < 30)
			std::cout << "Max word count too low generation may or may not stop too suddenly\n";

		else if (wordCount > 10000 && wordCount < 100000)
			std::cout << "Max word count very high, THIS WAS JUST A WARNING\n";


		else
			std::cout << "Max word count exceded 100000, THIS WAS JUST A WARNING\n";

	}

	

	namespace GenerationFail
	{
		void printContext(const std::vector<std::string>& strings)
		{
			std::cout << "FAILED ON: [ ";
			for (const auto& w : strings)
			{
				std::cout << w << " ";
			}
			std::cout << " ]\n";

		}



		void printContext(const std::pair<std::string, std::string>& strings)
		{
			std::cout << "FAILED ON: [" << strings.first << "] [" << strings.second << "]\n";

		}



		void printContext(const std::vector<char>& chars)
		{
			std::cout << "FAILED ON: [ ";
			for (const char& c : chars)
			{
				std::cout << c << " ";
			}
			std::cout << " ]\n";
		}



		void printContext(const std::pair<char, char>& chars)
		{
			std::cout << "FAILED ON: [" << chars.first << "] [" << chars.second << "]\n";
		}

	}



}





namespace UI::Vocab
{
	void Vocabulary(const VocabularyManager& vm)
	{
		UI::Common::Header::printSmallHeader("Vocabulary");


		if (vm.getAll().empty())
		{
			UI::Common::printEmpty("Vocabulary");

			return;
		}

		vm.displayVocabulary();
	}
}






