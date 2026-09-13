#pragma once
#include "AIContext.h"
#include "Types.h"





enum class TrainModel { NONE, Both, Legacy, NGram };

namespace Train
{
	std::vector<std::string> takeInputs();

	void recordTrainingInput(AIContext& context, const std::string& original, const std::string& normalized);

	//void selfTrain(AIContext& context, int amount);


	namespace Preparation
	{
		struct PreparedTrainingData
		{
			std::string originalText;
			std::string normalizedText;

			std::vector<WordData> analized;

			std::vector<std::string> tokens;


		};


		PreparedTrainingData prepareTrainingData(const std::string& text);

		TrainModel determineTrainingModel(std::string_view modelString);

	}



	void trainLegacy(AIContext& context, Preparation::PreparedTrainingData& data);

	void trainNGram(AIContext& context, Preparation::PreparedTrainingData& data, const size_t ngramSizeStart);





}



