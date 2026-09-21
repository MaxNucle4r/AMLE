#include "CommandClear.h"
#include "Option Parser.h"
#include "ModelTypeConversion.h"
#include "ModelType.h"
#include "UI.h"
#include "NGram.h"




namespace Execute::Clear::Model
{
	void clearModel(AIContext& ctx)
	{
		ctx.clearModel();
	}

}



namespace Execute::Clear::Memory
{
	void clearMemory(AIMemory& memory)
	{
		memory.clear();
	}

	void clearTrainingInputs(std::vector<TrainingInput>& trainingInputs)
	{
		trainingInputs.clear();
	}
}



namespace Execute::Clear::Vocabulary
{
	void clearVocabulary(VocabularyManager& vocabulary)
	{
		vocabulary.clear();
	}
}



namespace Execute::Clear::Debugger
{
	void clearDebugger(Debug& debugger)
	{
		debugger.disableAll();
	}
}





// CAREFUL U CANNOT USE THE NORMAL OPTION PARSER FUNCITONS
// as they need a default value.
// AND U DO NOT WANT TO CLEAR SOMEHTING ELSE 
namespace
{
	void clearAllProbabilities(AIContext& ctx)
	{
		Execute::Clear::Model::clearMap(ctx.getTextProbs());

		Execute::Clear::Model::clearMap(ctx.getCharProbs());

		for (auto& [size, model] : ctx.getAllNGrams())
		{
			Execute::Clear::Model::clearMap(model.getTextProbabilities());

			Execute::Clear::Model::clearMap(model.getCharProbabilities());
		}
			

	}


	void clearLegacyProbabilities(AIContext& ctx)
	{
		Execute::Clear::Model::clearMap(ctx.getTextProbs());

		Execute::Clear::Model::clearMap(ctx.getCharProbs());
	}



	void clearAllTransitions(AIContext& ctx)
	{
		Execute::Clear::Model::clearMap(ctx.getTextTransitions());
		Execute::Clear::Model::clearMap(ctx.getTextProbs());

		Execute::Clear::Model::clearMap(ctx.getCharTransitions());
		Execute::Clear::Model::clearMap(ctx.getCharProbs());


		ctx.clearAllNGrams();
	}


	void clearLegacyTransitions(AIContext& ctx)
	{
		Execute::Clear::Model::clearMap(ctx.getTextTransitions());
		Execute::Clear::Model::clearMap(ctx.getTextProbs());

		Execute::Clear::Model::clearMap(ctx.getCharTransitions());
		Execute::Clear::Model::clearMap(ctx.getCharProbs());
	}





	ModelType handleModelPicking(const ParsedCommand& cmd)
	{

		std::string modelS = OptionParser::getLowerCaseStringOption(cmd, "model");


		ModelType model = ModelTypeUtils::stringToModelType(modelS);

		if (model == ModelType::NONE)
			throw std::invalid_argument("Incorect Model Option");


		return model;
	}

	struct NgramModelToClear
	{
		bool provided = false;
		size_t size = 0;
	};

	NgramModelToClear handleNgramSizePicking(const ParsedCommand& cmd)
	{ 
		NgramModelToClear ngram;

		if (cmd.options.contains("ngram-size"))
		{
			ngram.size = OptionParser::getSizeOption(cmd, "ngram-size");
			ngram.provided = true;

			return ngram;
		}

		return ngram;

	}



	void clearTransitions(const ParsedCommand& cmd, AIContext& ctx)
	{
		if (cmd.options.empty())
		{
			
			clearAllTransitions(ctx);

			return;

		}

		

		ModelType model = handleModelPicking(cmd);

		if (model == ModelType::Legacy)
		{
			clearLegacyTransitions(ctx);
			return;
		}


		if (model == ModelType::NGram)
		{

			auto ngramStats = handleNgramSizePicking(cmd);

			if (!ngramStats.provided)
				ctx.clearAllNGrams();

			else
			{
				ctx.clearNGram(ngramStats.size);

				return;

			}
				


		}


		throw std::invalid_argument("Incorrect model option");


	}



	void clearProbabilities(const ParsedCommand& cmd, AIContext& ctx)
	{
		if (cmd.options.empty())
		{

			clearAllProbabilities(ctx);

			return;

		}



		ModelType model = handleModelPicking(cmd);

		if (model == ModelType::Legacy)
		{
			clearLegacyProbabilities(ctx);
			return;
		}


		if (model == ModelType::NGram)
		{

			auto ngramStats = handleNgramSizePicking(cmd);

			if (!ngramStats.provided)
				ctx.clearAllNGrams();

			else
			{
				ctx.getNGram(ngramStats.size).getCharProbabilities().clear();
				ctx.getNGram(ngramStats.size).getTextProbabilities().clear();


				return;

			}



		}


		throw std::invalid_argument("Incorrect model option");


	}
	

}



namespace Execute::Clear
{
	void cleaner(const ParsedCommand& cmd, AIContext& ctx)
	{
		switch (cmd.target)
		{
			case Target::Transitions:
				clearTransitions(cmd, ctx);
				break;



			case Target::Probs:
				clearProbabilities(cmd, ctx);
				break;



			case Target::Self:
				Execute::Clear::Model::clearModel(ctx);
				break;



			case Target::Memory:
				Execute::Clear::Memory::clearMemory(ctx.getMemory());
				Execute::Clear::Memory::clearTrainingInputs(ctx.getTrainingInputs());
				break;



			case Target::Vocab:
				Execute::Clear::Vocabulary::clearVocabulary(ctx.getVocabulary());
				break;


			case Target::DebugFlags:
				Execute::Clear::Debugger::clearDebugger(ctx.getDebugger());
				break;



			default:
				AMLE_ASSERT(false);
				throw std::runtime_error("Invalid Clear Target");
		}
	}
}