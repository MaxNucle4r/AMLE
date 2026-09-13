#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <format>

#include "Similarity.h"
#include "UI.h"





namespace Compare
{
	double wordOrderSimilarity(std::string_view a, std::string_view b)
	{
		int matches = 0;
		std::string word;

		std::vector<std::string> words_a;
		std::vector<std::string> words_b;


		std::stringstream ssa{ std::string(a) };
		while (ssa >> word)
			words_a.push_back(word);


		std::stringstream ssb{ std::string(b) };
		while (ssb >> word)
			words_b.push_back(word);


		size_t size = std::min(words_a.size(), words_b.size());

		for (size_t i = 0; i < size; ++i)
		{
			if (words_a[i] == words_b[i])
				++matches;

		}


		size_t error = std::max(words_a.size(), words_b.size()) - size;


		double similarity = matches * 100.0 / size;

		double lengthPenalty =
			error * 100.0 / std::max(words_a.size(), words_b.size());

		similarity -= lengthPenalty * 0.5;


		return similarity;

	}




	bool checkSimilarity(std::string_view normInput, std::string_view original,
		const std::vector<TrainingInput>& entries)
	{

		std::string response;

		//look for 1:1 match first... 
		// just in case
		// to skip the rest of the code

		// SYNTAX
		//[]        // Capture nothing.
		//[&]       // Capture everything you use by reference.
		//[=]       // Capture everything you use by value (copy).
		//[&x]      // Capture only x by reference.
		//[x]       // Capture only x by value.
		//[&, x]    // Everything else by reference, x by value.
		//[=, &x]   // Everything else by value, x by reference.
		// 
		//auto f = [=]() mutable
		//[=] captures a copy and it is treated as const
		// mutable lets u overwrite that value

		auto found = std::find_if(entries.begin(), entries.end(),
			[normInput, original](const TrainingInput& t)
			{
				return t.getNormalized() == normInput
					|| t.getOriginal() == original;
			});



		if (found != entries.end())
		{
			UI::Warning::printWarningText("Perfect match of training input was found in previous training inputs, still submit ? y/n   ");

			std::getline(std::cin, response);

			return response == "y" || response == "Y" ;
		}


		else
		{
			for (const auto& e : entries)
			{
				double normFamiliarity = Compare::wordOrderSimilarity(e.getNormalized(), normInput);
				double originalFamiliarity = Compare::wordOrderSimilarity(e.getOriginal(), original);

				if (normFamiliarity >= 95.0 ||
					originalFamiliarity >= 95.0)
				{

					double similarity = std::max(normFamiliarity, originalFamiliarity);


					UI::Warning::printWarningText(
						"A very similar training input was found!\n\n"
						"New input:\n" +
						std::string(original) +
						"\n\nExisting input:\n" +
						e.getOriginal() +

						std::format(
							"Similarity: ***{:.2f}%***\n\n"
							"Still submit? y / n",
							similarity
						) +

						" % *** still submit ? y / n");

					std::getline(std::cin, response);

					return response == "y" || response == "Y" ;

				}

			}
		}


		return true;

	}



}