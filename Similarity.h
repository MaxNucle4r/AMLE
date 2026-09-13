#pragma once
#include <string_view>

#include "TrainingInput.h"





namespace Compare
{
	double wordOrderSimilarity(std::string_view a, std::string_view b);

	bool checkSimilarity(std::string_view normInput, std::string_view original,
		const std::vector<TrainingInput>& entries);

	//double embedingsSimilarity()

	//whatever else?

}

