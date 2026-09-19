#include "CommandTraining.h"
#include "Training.h"
#include "Option Parser.h"
#include "Similarity.h"





struct TrainModelStats
{
	TrainModel model = TrainModel::Both;
	size_t trainableNgramModels = 0;
	std::vector<std::string> trainingInputs;
};

static TrainModelStats createTrainingPlan(const ParsedCommand& cmd)
{
	TrainModelStats stats;



	// the options... think about how u want them to work...
	// decide on what the command for training should look like?
	// IDK... -_-
	// @train>input{model = ngram } should train all ngrams?
	// @train>input{model = ngram/ size = 4} should train ngrams <=4
	// @train>input{} / @train>input should train everything
	//  maybe also allow somehting like @train>input{model-size = 4} so it train legacy and ngrams <=4 ?
	//  bruh.
	// allow both @train>input{model = both} and @train>input{} to be also training both 
	if (cmd.options.contains("model"))
	{
		std::string modelString =
			OptionParser::getLowerCaseStringOption(cmd, "model", "");

		stats.model = Train::Preparation::determineTrainingModel(modelString);


	}

	else
	{
		stats.model = TrainModel::Both;
	}





	if (stats.model == TrainModel::Both)
	{
		UI::Common::printText("No model specified. Both will be trained\n");
		UI::Common::printText("All recommended NGram models will be trained: "
			+ std::to_string(NgramModel::RECOMMENDED_MAX_CONTEXT_SIZE) + "\n");


		stats.trainableNgramModels = NgramModel::RECOMMENDED_MAX_CONTEXT_SIZE;

	}



	else if (stats.model == TrainModel::NGram)
	{
		UI::Common::printText("Only NGram models will be trained\n");


		//maybe another name rather than  "model - size" ?
		// maybe just "models = [number]"
		if (cmd.options.contains("ngram-size"))
		{
			stats.trainableNgramModels =
				OptionParser::getSizeOption(cmd, "ngram-size", 0);


			NgramModel::validateContextSize(stats.trainableNgramModels);


			AMLE_ASSERT(stats.trainableNgramModels >= NgramModel::MIN_CONTEXT_SIZE);
			AMLE_ASSERT(stats.trainableNgramModels <= NgramModel::MAX_CONTEXT_SIZE);


		}

		else
		{
			UI::Common::printText("All recommended NGram models will be trained: "
				+ std::to_string(NgramModel::RECOMMENDED_MAX_CONTEXT_SIZE) + "\n");


			stats.trainableNgramModels = NgramModel::RECOMMENDED_MAX_CONTEXT_SIZE;

		}

	}



	else if (stats.model == TrainModel::Legacy)
	{
		UI::Common::printText("Only Legcay model will be trained\n");

	}



	else
	{
		AMLE_ASSERT(false);
	}


	//maybe rework this later.
	// idk for now oughtta do it
	//u alrdy changed this part 6 times...
	stats.trainingInputs = Train::takeInputs();



	return stats;


}



static void printTokens(const std::vector<std::string>& tokens)
{
	std::cout << "***TOKENS: ***\n";

	int i = 0;
	for (const auto& s : tokens)
	{
		std::cout << i << ": " << s << "\n";
		++i;
	}

	UI::spacer();
}


static void printNormalizedText(const std::string_view norm)
{
	std::cout << "\n***NORMALIZED***\n";

	std::cout << norm << "\n";


	UI::spacer();

}



namespace Execute::Training
{
	void initiateTraining(AIContext& ctx, const ParsedCommand& cmd)
	{


		TrainModelStats modelStats = createTrainingPlan(cmd);


		for (const std::string& input : modelStats.trainingInputs)
		{
			auto data = Train::Preparation::prepareTrainingData(input);



			if (ctx.getDebugger().enabled(DebugFlags::Training))
			{
				printNormalizedText(data.normalizedText);
				printTokens(data.tokens);

			}


			if (!Compare::checkSimilarity(data.normalizedText, data.originalText, ctx.getTrainingInputs()))
				continue;


			Train::recordTrainingInput(ctx, data.originalText, data.normalizedText);


			if (ctx.getDebugger().enabled(DebugFlags::Training))
			{
				std::cout << "Training input database size: "
					<< ctx.getTrainingInputs().size()
					<< "\n";
			}



			switch (modelStats.model)
			{
			case TrainModel::Legacy:
				Train::trainLegacy(ctx, data);
				break;


			case TrainModel::NGram:
				Train::trainNGram(ctx, data, modelStats.trainableNgramModels);
				break;


			case TrainModel::Both:
				Train::trainLegacy(ctx, data);
				Train::trainNGram(ctx, data, modelStats.trainableNgramModels);
				break;


			default:
				AMLE_ASSERT(false);
				throw std::logic_error("UNEXPECTED TRAINING MODEL");
			}



		}



	}


}
