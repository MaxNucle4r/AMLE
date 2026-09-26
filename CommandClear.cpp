#include "CommandClear.h"
#include "Option Parser.h"
#include "ModelTypeConversion.h"
#include "ModelType.h"
#include "UI.h"
#include "NGram.h"
#include "CommandUtils.h"
#include "ValidationTemplatesh.h"




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



/*
namespace Execute::Clear::Debugger
{
	void clearDebugger(Debug& debugger)
	{
		debugger.disableAll();
	}
}
*/





// CAREFUL U CANNOT USE THE NORMAL OPTION PARSER FUNCITONS
// as they need a default value.
// AND U DO NOT WANT TO CLEAR SOMEHTING ELSE 
namespace
{
	void clearAllProbabilities(AIContext& ctx)
	{
		Execute::Clear::clearMap(ctx.getTextProbs());

		Execute::Clear::clearMap(ctx.getCharProbs());

		for (auto& [size, model] : ctx.getAllNGrams())
		{
			Execute::Clear::clearMap(model.getTextProbabilities());

			Execute::Clear::clearMap(model.getCharProbabilities());
		}
			

	}


	void clearLegacyProbabilities(AIContext& ctx)
	{
		Execute::Clear::clearMap(ctx.getTextProbs());

		Execute::Clear::clearMap(ctx.getCharProbs());
	}



	void clearAllModels(AIContext& ctx)
	{
		Execute::Clear::clearMap(ctx.getTextTransitions());
		Execute::Clear::clearMap(ctx.getTextProbs());

		Execute::Clear::clearMap(ctx.getCharTransitions());
		Execute::Clear::clearMap(ctx.getCharProbs());


		ctx.clearAllNGrams();
	}


	void clearLegacy(AIContext& ctx)
	{
		Execute::Clear::clearMap(ctx.getTextTransitions());
		Execute::Clear::clearMap(ctx.getTextProbs());

		Execute::Clear::clearMap(ctx.getCharTransitions());
		Execute::Clear::clearMap(ctx.getCharProbs());
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



	bool clearTransitions(const ParsedCommand& cmd, AIContext& ctx)
	{
		if (!cmd.options.contains("model"))
		{
			//dont even bother to check for empty models xD
			// 	
			clearAllModels(ctx);

			return true;

		}

		

		ModelType model = handleModelPicking(cmd);

		if (model == ModelType::Legacy)
		{
			if (ctx.isLegacyEmpty())
			{
				UI::Warning::printWarningText(
					"Legacy is already empty"
				);


				return false;
			}
				

			clearLegacy(ctx);

			return true;
		}


		if (model == ModelType::NGram)
		{

			auto ngramStats = handleNgramSizePicking(cmd);

			if (!ngramStats.provided)
			{
				ctx.clearAllNGrams();

				return true;
			}
				

			else
			{
				if (Validation::isEmpty(ctx.getNGram(ngramStats.size)))
				{

					UI::Warning::printWarningText(
						"Ngram with size: " +
						std::to_string(ngramStats.size) +
						" is already empty"
					);


					return false;
				}

				ctx.clearNGram(ngramStats.size);

				return true;

			}
				


		}


		throw std::invalid_argument("Incorrect model option");


	}



	bool clearProbabilities(const ParsedCommand& cmd, AIContext& ctx)
	{
		//dont even bother to check for empty probs xD
		if (!cmd.options.contains("model"))
		{

			clearAllProbabilities(ctx);

			return true;

		}



		ModelType model = handleModelPicking(cmd);

		if (model == ModelType::Legacy)
		{
			clearLegacyProbabilities(ctx);

			return true;
		}


		if (model == ModelType::NGram)
		{

			auto ngramStats = handleNgramSizePicking(cmd);

			if (!ngramStats.provided)
			{
				
				for (auto& [size, ngram] : ctx.getAllNGrams())
				{
					Execute::Clear::clearMap(ngram.getCharProbabilities());
					Execute::Clear::clearMap(ngram.getTextProbabilities());
				}

				return true;
			}

			else
			{
				if (Validation::isEmpty(ctx.getNGram(ngramStats.size)))
				{
					UI::Warning::printWarningText(
						"Ngram with size: " +
						std::to_string(ngramStats.size) +
						" is already empty");

					return false;
				}



				auto& ngram = ctx.getNGram(ngramStats.size);

				Execute::Clear::clearMap(
					ngram.getTextProbabilities());

				Execute::Clear::clearMap(
					ngram.getCharProbabilities());



				return true;

			}


		}


		throw std::invalid_argument("Incorrect model option");


	}
	

}



namespace
{
	std::string clearTransitionsProbs(const ParsedCommand& cmd)
	{

		std::string message;

		if (cmd.target == Target::Transitions ||
			cmd.target == Target::Probs)
		{
			if (!cmd.options.contains("model"))
				message += "for ALL MODELS ";

			else
			{
				ModelType model = handleModelPicking(cmd);


				message += "for model: " +
					ModelTypeUtils::modelTypeToString(model) + " ";


				if (model == ModelType::NGram)
				{
					if (!cmd.options.contains("ngram-size"))
						message += "ALL NGRAM sizes";

					else
					{
						message += "size: " +
							cmd.options.at("ngram-size");
					}

				}


			}



		}


		return message;


	}

}

namespace Execute::Clear
{
	std::string buildCleanerWarning(const ParsedCommand& cmd)
	{
		std::string message=
			"Are you sure you want to clear ";

		std::string endMessage =
			"This action *CANNOT* be undone... yet.";



		std::string target =
			std::string{
			CommandUtils::Targets::targetToString(cmd.target)};


		if (cmd.target == Target::Self)
			target = "AMLE";


		message += target + " " + clearTransitionsProbs(cmd);

		message += "\n" + endMessage + " [Y/N]";
		


		return message;

	}



	bool checkCleaner(const ParsedCommand& cmd)
	{

		bool warn = OptionParser::getBoolOption(cmd, "warn", true);

		if (warn == false)
			return true;



		std::string message = buildCleanerWarning(cmd);

		UI::Common::printText(message);

		std::string confirm;

		std::getline(std::cin, confirm);

		if (confirm == "y" || confirm == "Y")
			return true;

		else if (confirm == "n" || confirm == "N")
			return false;

		else if (confirm.empty())
			throw std::invalid_argument("You must confirm");


		throw std::invalid_argument("Invalid Confirmation");

	}

}



namespace
{
	// true  = something exists, proceed with clear
	// false = already empty, stop
	bool checkEmpty(const ParsedCommand& cmd, AIContext& ctx)
	{
		switch (cmd.target)
		{

			case Target::Self:
			{
				if (Validation::isEmpty(ctx))
				{
					UI::Warning::printWarningText("AMLE is already empty");
					
					return false;
				
				}

				return true;
				
			}




			case Target::Memory:
			{
				bool memoryEmpty =
					Validation::isEmpty(ctx.getMemory());

				bool trainingInputsEmpty =
					ctx.getTrainingInputs().empty();



				if (memoryEmpty && trainingInputsEmpty)
				{
					UI::Warning::printWarningText(
						"Memory and Training Inputs are already empty");

					return false;
				}

				if (memoryEmpty)
				{
					UI::Warning::printWarningText(
						"Memory is already empty");
				}

				if (trainingInputsEmpty)
				{
					UI::Warning::printWarningText(
						"Training Inputs are already empty");
				}


				return true;
			}



			case Target::Vocab:
			{
				if (Validation::isEmpty(ctx.getVocabulary()))
				{
					UI::Warning::printWarningText(
						"Vocabulary is already empty");

					return false;
				}

				return true;
			}


		}


		return true;

	}

}



namespace Execute::Clear
{
	void cleaner(const ParsedCommand& cmd, AIContext& ctx)
	{


		if (!checkEmpty(cmd, ctx))
			return;



		if (!checkCleaner(cmd))
			return;



		switch (cmd.target)
		{
			case Target::Transitions:
				if (!clearTransitions(cmd, ctx))
					return;
				break;



			case Target::Probs:
				if(!clearProbabilities(cmd, ctx))
					return;
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

			
			/*
			TO DO
			find a purpose for this xD
			case Target::DebugFlags:
				Execute::Clear::Debugger::clearDebugger(ctx.getDebugger());
				break;
			*/



			default:
				AMLE_ASSERT(false);
				throw std::runtime_error("Invalid Clear Target");
		}





		UI::Common::printText("Clear Successful");

		UI::spacer();

	}


}