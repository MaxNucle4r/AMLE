#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <utility>

#include "Training.h"
#include "Model.h"
#include "TrainingInput.h"
#include "Model Builder.h"





static void checkFailedInput(const TextProcess& process)
{

	AMLE_ASSERT(process != TextProcess::PASSED);


	switch (process)
	{


	case TextProcess::EMPTY:
		UI::Warning::printWarningText("Empty input");
		break;


	case TextProcess::ONLY_SPACES:
		UI::Warning::printWarningText("Only spaces detected, try agian");
		break;


	case TextProcess::ONLY_NUMBERS:
		UI::Warning::printWarningText("Only numbers detected, try again");
		break;


	case TextProcess::ONLY_SYMBOLS:
		UI::Warning::printWarningText("Only symbols detected, try agian");
		break;


	default:
		AMLE_ASSERT(false);
		UI::Warning::printWarningText("Unknow error has occured, try agian");
		break;

	}
}


namespace Train
{
	void recordTrainingInput(AIContext& context, const std::string& original, const std::string& normalized)
	{
		auto& entries = context.getTrainingInputs();


		auto found = std::find_if(entries.begin(), entries.end(),
			[&original](const TrainingInput& t) {return t.getOriginal() == original; });


		if (found != entries.end())
		{

			found->incrementSeen();
			found->updateLastSeen();

		}

		else
		{
			entries.emplace_back(original, normalized);
		}



	}
}





namespace Train
{

	//either make this a std::vector<std::string> takeInputs
	// or make void trainLoop() where u call takeInput/ takeInputs then call whatever training function is needed
	//or keep the current std::stringTakeInput
	//congrats u officially changed this funciton 6 times
	std::vector<std::string> takeInputs()
	{

		std::vector<std::string> inputs;
	
		while (true)
		{
			std::string input;

			std::cout << "ENTER A TEXT: ";
			std::getline(std::cin, input);


			//instead of using input[0]=='@'
			//maybe .starts_with() works?
			if (input == "stop" || input.starts_with('@') || input.empty())
				return inputs;



			TextProcess process = TextProcess::NONE;
			bool passed = TextProcessing::Validate(input, process);


			if (!passed)
			{
				checkFailedInput(process);
				continue;

			}


			inputs.push_back(input);
		}


	}





	namespace Preparation
	{
		PreparedTrainingData prepareTrainingData(const std::string& text)
		{
			PreparedTrainingData data;

			data.originalText = text;
			data.analized = TextProcessing::Analyze(text);
			data.tokens = TextProcessing::Tokenize(data.analized);
			data.normalizedText = TextProcessing::RebuildText(data.analized);



			return data;

		}


		TrainModel determineTrainingModel(std::string_view modelString)
		{
			if (modelString == "ngram")
				return TrainModel::NGram;

			else if (modelString == "legacy")
				return TrainModel::Legacy;

			else if (modelString == "both")
				return TrainModel::Both;

			else
				return TrainModel::NONE;


		}

		

	}





	void trainLegacy(AIContext& context, 
		Preparation::PreparedTrainingData& data)
	{


		Model::Text::buildTextTransitions(context.getTextTransitions(), data.tokens);
		Model::Word::buildCharTransitions(context.getCharTransitions(), data.tokens);


		// either calculate probabilities here
		// or at the end of everything
		context.getTextProbs() = Model::Text::makeProbabilities(context.getTextTransitions());
		context.getCharProbs() = Model::Char::makeProbabilities(context.getCharTransitions());


	}





	void trainNGram(AIContext& context, 
		Preparation::PreparedTrainingData& data, 
		const size_t modelsToTrain)
	{


		



		// TO DO
		//maybe also make an overload that takes a std::vector<int> modelSizesToTrain
		// once u can do @train>input{model = ngram/ size=4,3,2}
		// this is a map not unordered_map
		// either calculate probabilities here
		// or at the end of everything
		if (context.getDebugger().enabled(DebugFlags::Training))
		{
			UI::Common::Header::printSmallHeader("Enterring NGram Transitions update loop");
		}

		for (auto& [size, model] : context.getAllNGrams())
		{

			if (size > modelsToTrain)
				break;



			if (context.getDebugger().enabled(DebugFlags::Training))
			{

				UI::Common::printText("Current Size: " + std::to_string(size));	

				std::cout
					<< "Model address: "
					<< &model
					<< '\n';
			}

			

			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::printText("Text transition map size before map update: " + std::to_string(model.getTextTransitions().size()));
			
			Model::Text::buildTextTransitionsNGram(model.getTextTransitions(), size, data.tokens);
			
			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::printText("Text Transition map size after map update: " + std::to_string(model.getTextTransitions().size()));



			model.getTextProbabilities() =
				Model::Text::makeProbabilitiesNGram(model.getTextTransitions());



			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::Header::printSmallHeader("Text Transitions + Probs Finished for size: " + std::to_string(size));


			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::printText("Char transition map size before map update: " + std::to_string(model.getCharTransitions().size()));
			
			Model::Word::buildCharTransitionsNGram(model.getCharTransitions(), size, data.tokens);
			
			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::printText("Char transition map size after map update: " + std::to_string(model.getCharTransitions().size()));


			model.getCharProbabilities() =
				Model::Char::makeProbabilitiesNGram(model.getCharTransitions());



			if (context.getDebugger().enabled(DebugFlags::Training))
				UI::Common::Header::printSmallHeader("Char Transitions + Probs Finished for size: " + std::to_string(size));


		}


		if (context.getDebugger().enabled(DebugFlags::Training))
			UI::Common::Header::printSmallHeader("NGram Training function ENDED");

	}


}

