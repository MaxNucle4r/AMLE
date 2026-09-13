#include <optional>
#include <iostream>
#include <algorithm>
#include <cctype>



#include "Command Executor.h"
#include "UI.h"
#include "Generation.h"
#include "Score.h"
#include "Persistence.h"
#include "FileSystem.h"
#include "TimeStamp.h"
#include "Option Parser.h"
#include "Configuration.h"
#include "Training.h"
#include "Version.h"

#include "ValidationTemplatesh.h"
#include "Similarity.h"
#include "ModelTypeConversion.h"










namespace GenerationHelpers
{

	ModelType pickModelType(const ModelType& defaultModel,
		const ParsedCommand& cmd)
	{

		std::string defaultModelString = ModelTypeUtils::modelTypeToString(defaultModel);



		std::string userModelString =
			OptionParser::getLowerCaseStringOption(cmd, "model", defaultModelString);



		ModelType model = ModelTypeUtils::stringToModelType(userModelString);



		return model;


	}



	std::optional<std::vector<std::string>> pickGenerationSeed(const ParsedCommand& cmd, 
		const Debug& debugger)
	{
		std::optional<std::vector<std::string>> seed;


		std::string seedStringLC =
			OptionParser::getLowerCaseStringOption(cmd, "seed", "");



		if (!seedStringLC.empty())
		{
			seed = Seed::extractSeed(seedStringLC);


			UI::spacer();

			std::cout << "SEED TAKEN: ";

			if (debugger.enabled(DebugFlags::Generation))
			{
				for (const auto& word : *seed)
					std::cout << word << " ";

				UI::spacer();
			}



			std::string originalSeed =
				OptionParser::getOriginalStringOption(cmd, "seed", "");

			std::cout << originalSeed;


			UI::spacer();

		}


		return seed;

	}



	size_t pickModelTypeSize(const ParsedCommand& cmd,const size_t defaultSize)
	{

		size_t modelSize =
			OptionParser::getSizeOption(cmd, "ngram-size", defaultSize);


		NgramModel::validateContextSize(modelSize);


		AMLE_ASSERT(modelSize >= NgramModel::MIN_CONTEXT_SIZE);
		AMLE_ASSERT(modelSize <= NgramModel::MAX_CONTEXT_SIZE);


		return modelSize;

	}



	void handleGenerationDebug(const Debug& debugger, const std::string& reply, const EndingType& endtype, VocabularyManager& vm)
	{
		if (debugger.enabled(DebugFlags::Generation))
		{
			std::cout << (reply.empty() ? "BUGGED" : "");
			std::cout << "\n---REPLY SIZE: " << reply.size() << "---\n";

			std::cout << "STOPPED GENERATING DUE TO: " << GenerationHelpersToString::EndingTypeToString(endtype);
			UI::spacer();

		}


		if (debugger.enabled(DebugFlags::Scoring))
		{
			bool ignoreLenghtPenalty = true;

			float score = Scoring::Text::score(reply, vm, ignoreLenghtPenalty, debugger);


			std::cout << "SCORE: " << score << "\n";
			UI::spacer();

		}
	}


}




namespace Execute::Generate
{
	void Text(const ParsedCommand& cmd, AIContext& ctx)
	{

		std::optional<std::vector<std::string>> seed;



		double temperature =
			OptionParser::getDoubleOption(cmd, "temp", ctx.getConfig().getTemp());


		//does nothing for now
		// will be taken in words
		/*
		size_t textLength =
			OptionParser::getSizeOption(cmd, "text-length", 50);
		*/


		
			

		size_t modelSize = GenerationHelpers::pickModelTypeSize(cmd, ctx.getConfig().getDefaultNGramSize());
		

		ModelType model = GenerationHelpers::pickModelType(ctx.getConfig().getDefaultModel(), cmd);


		seed = GenerationHelpers::pickGenerationSeed(cmd, ctx.getDebugger());



		GenerationType gentype = GenerationType::NONE;
		EndingType endtype = EndingType::NONE;




		std::string reply; 
		
		if (model == ModelType::Legacy)
			reply = Generation::Text(ctx.getTextProbs(), temperature, endtype, gentype, seed, ctx.getDebugger());

		else if (model == ModelType::NGram)
			reply = Generation::Text(ctx.getNGram(modelSize).getTextProbabilities(), temperature, endtype, gentype, seed, ctx.getDebugger(), modelSize);



		GenerationHelpers::handleGenerationDebug(ctx.getDebugger(), reply, endtype, ctx.getVocabulary());

		

		std::cout << reply;

		ctx.getMemory().add("", reply);

		UI::spacer();



	}





	//TO DO
	// finish word generation
	// like u did with the text
	void Word(const ParsedCommand& cmd, AIContext& ctx)
	{
		double temperature =
			OptionParser::getDoubleOption(cmd, "temp", 1.0);


		//does nothing for now
		// will be taken in characters
		/*
		size_t wordLength =
			OptionParser::getSizeOption(cmd, "word-length", 20);
		*/


		GenerationType gentype = GenerationType::NONE;
		EndingType endtype = EndingType::NONE;

		std::string reply = Generation::Word(ctx.getCharProbs(), temperature, endtype, gentype, ctx.getDebugger());


		if (ctx.getDebugger().enabled(DebugFlags::Generation))
		{
			std::cout << (reply.empty() ? "BUGGED" : "");
			std::cout << "\n---REPLY SIZE: " << reply.size() << "---\n";

			std::cout << "STOPPED GENERATING DUE TO: " << GenerationHelpersToString::EndingTypeToString(endtype);
			UI::spacer();
		}


		if (ctx.getDebugger().enabled(DebugFlags::Scoring))
		{
			bool ignoreLenghtPenalty = true;

			float score = Scoring::Text::score(reply, ctx.getVocabulary(), ignoreLenghtPenalty, ctx.getDebugger());


			std::cout << "SCORE: " << score << "\n";
			UI::spacer();

		}

		std::cout << reply;

		ctx.getMemory().add("", reply);

		UI::spacer();



	}


}



namespace Execute::Print
{
	void TextTransitions(const AIContext& ctx)
	{
		UI::Legacy::printTransitions(ctx.getTextTransitions());
	}

	void TextProbabilities(const AIContext& ctx)
	{
		UI::Legacy::printProbabilities(ctx.getTextProbs());
	}



	void NGramTextTransitions(const AIContext& ctx, const size_t size)
	{
		UI::NGram::printTransitions(ctx.getNGram(size).getTextTransitions());
	}

	void NGramTextProbabilities(const AIContext& ctx, const size_t size)
	{
		UI::NGram::printProbabilities(ctx.getNGram(size).getTextProbabilities());
	}





	void CharTransitions(const AIContext& ctx)
	{
		UI::Legacy::printTransitions(ctx.getCharTransitions());
	}

	void CharProbabilities(const AIContext& ctx)
	{
		UI::Legacy::printProbabilities(ctx.getCharProbs());
	}



	void NGramCharTransitions(const AIContext& ctx, const size_t size)
	{
		UI::NGram::printTransitions(ctx.getNGram(size).getCharTransitions());
	}

	void NGramCharProbabilities(const AIContext& ctx, const size_t size)
	{
		UI::NGram::printProbabilities(ctx.getNGram(size).getCharProbabilities());
	}

	




	// TO DO
	// rework this to make it cleaner
	// it can accept both -10 and 10 as values
	// 10 = first 10
	// -10 = last 10
	void Memory(const ParsedCommand& cmd, const AIContext& ctx)
	{
		int size = 0;

		if (auto size_it = cmd.options.find("size");
			size_it != cmd.options.end())
		{
			size = std::stoi(size_it->second);
			
			if (size == 0)
				throw std::out_of_range("Size must be a valid non zero number");

		}


		size_t amount = static_cast<size_t>(std::abs(size));
		size_t absSize = static_cast<size_t>(std::abs(size));

		if (absSize > ctx.getMemory().size())
		{
			std::cout << "Range out of bounds \n";
		}
			

		amount = std::min(amount, ctx.getMemory().size());

		if (amount != absSize)
			std::cout << " 'Size' has been set to: " << amount << "\n";



		if (size < 0)
			UI::Memory::printMemory(ctx.getMemory().getLast(amount));

		else if (size > 0)
			UI::Memory::printMemory(ctx.getMemory().getFirst(amount));

		else
			UI::Memory::printMemory(ctx.getMemory().getAll());

	
	}



	void Vocabulary(const AIContext& ctx)
	{
		UI::Vocab::Vocabulary(ctx.getVocabulary());
	}


}






struct PrintedModelStats
{
	ModelType model;
	size_t size;
};


static PrintedModelStats chooseModelForPrinting(const ModelType& defaultModel,
	const size_t defaultModelSize,
	const ParsedCommand& cmd)
{
	std::string modelString =
		OptionParser::getLowerCaseStringOption(cmd, "model");

	ModelType model = ModelTypeUtils::stringToModelType(modelString);
	
	if (model == ModelType::NONE)
	{
		UI::Warning::invalidOption("model", modelString, ModelTypeUtils::modelTypeToString(defaultModel));

		model = defaultModel;
	}


	size_t modelSize = 0;


	if (model == ModelType::NGram)
	{
		modelSize = 
			OptionParser::getSizeOption(cmd, "ngram-size", defaultModelSize);

		modelSize = Validation::clampWithWarning("ngram-size", modelSize, NgramModel::MIN_CONTEXT_SIZE, NgramModel::MAX_CONTEXT_SIZE);

	}
		


	return PrintedModelStats
	{
		.model = model,
		.size = modelSize
	};


}



struct PrintOptions
{
	bool text = true;
	bool character = true;

};

static PrintOptions getPrintOptions(const ParsedCommand& cmd)
{
	bool hasText = cmd.options.contains("text");
	bool hasChar = cmd.options.contains("char");

	PrintOptions options;


	if (!hasText && !hasChar)
		return options;



	if (hasText)
	{
		options.text = OptionParser::getBoolOption(
			OptionParser::getLowerCaseStringOption(cmd, "text", "")
		);
	}


	if (hasChar)
	{
		options.character = OptionParser::getBoolOption(
			OptionParser::getLowerCaseStringOption(cmd, "char", "")
		);
	}


	if (!options.text && !options.character)
	{
		throw std::invalid_argument(
			"At least one transition type must be true."
		);
	}



	if (hasText && !hasChar)
	{
		options.character = !options.text;
	}

	if (hasChar && !hasText)
	{
		options.text = !options.character;
	}



	return options;
}

static void handleLegacyModelPrinting(const ParsedCommand& cmd, const AIContext& ctx)
{
	PrintOptions options = getPrintOptions(cmd);



	if (cmd.target == Target::Transitions)
	{
		if (options.text)
			Execute::Print::TextTransitions(ctx);

		if (options.character)
			Execute::Print::CharTransitions(ctx);
	}

	else if (cmd.target == Target::Probs)
	{
		if (options.text)
			Execute::Print::TextProbabilities(ctx);

		if (options.character)
			Execute::Print::CharProbabilities(ctx);
	}

}


static void handleNGramModelPrinting(const ParsedCommand& cmd, const AIContext& ctx, const size_t modelSize)
{
	PrintOptions options = getPrintOptions(cmd);



	if (cmd.target == Target::Transitions)
	{
		if (options.text)
			Execute::Print::NGramTextTransitions(ctx, modelSize);

		if (options.character)
			Execute::Print::NGramCharTransitions(ctx, modelSize);
	}

	
	else if (cmd.target == Target::Probs)
	{
		if (options.text)
			Execute::Print::NGramTextProbabilities(ctx, modelSize);

		if (options.character)
			Execute::Print::NGramCharProbabilities(ctx, modelSize);
	}
	


}


static bool handleGenerincPrinting(const ParsedCommand& cmd,
	const AIContext& ctx)
{
	if (cmd.target == Target::Transitions)
	{
		if (cmd.options.empty())
		{
			Execute::Print::TextTransitions(ctx);

			Execute::Print::NGramTextTransitions(ctx, ctx.getConfig().getDefaultNGramSize());


			Execute::Print::CharTransitions(ctx);

			Execute::Print::NGramCharTransitions(ctx, ctx.getConfig().getDefaultNGramSize());

			return true;
		}
	}

	if (cmd.target == Target::Probs)
	{
		if (cmd.options.empty())
		{
			//NGRAM PROBS PRINTER NOT DONE

			Execute::Print::TextProbabilities(ctx);

			Execute::Print::CharProbabilities(ctx);

			return true;
		}
	}

	return false;

}



static void choosePrinter(const ParsedCommand& cmd,
	const AIContext& ctx)
{
	
	if (handleGenerincPrinting(cmd, ctx))
	{
		return;
	}
		

	
	PrintedModelStats model = chooseModelForPrinting(
		ctx.getConfig().getDefaultModel(), ctx.getConfig().getDefaultNGramSize(), 
		cmd);



	AMLE_ASSERT(
		model.model == ModelType::Legacy ||
		model.model == ModelType::NGram
	);


	if (model.model == ModelType::Legacy)
	{
		handleLegacyModelPrinting(cmd, ctx);
	}

	else if (model.model == ModelType::NGram)
	{
		handleNGramModelPrinting(cmd, ctx, model.size);
	}

			
}






static std::string getFileName(const ParsedCommand& cmd)
{
	std::string filename; 

	auto files = Persistence::Files::getFiles();


	auto file = cmd.options.find("filename");

	if (cmd.target == Target::Save && file == cmd.options.end())
	{
		filename = Persistence::Files::getDefaultName();

		std::cout << "No filename provided for saving\n"
			<< "Default name will be chosen instead\n"
			<< filename << "\n";
	}

	else if (cmd.target == Target::Save && file != cmd.options.end())
	{
		filename = file->second;


		if (auto it = std::find(files.begin(), files.end(), filename);
			it != files.end())
		{
			throw std::runtime_error("File with name: *" + filename + "* already exists");
		}

	}



	if (cmd.target == Target::Load && file == cmd.options.end())
	{
		//take the latest one ... hopefully xD
		//seems like... windows is not such a nice guy
		//it will get the files
		//just not in a proper order
		//fine for now...

		if (files.empty())
			throw std::runtime_error("No saved models found.");


		filename = files.back();

		//MUST change this later
		//u added j["metadata"]["lastUsed"]
		//for save format 1... good luck 
		//for save format 2 use that
		std::cout << "NO filename provided for loading\n"
			<< "Using an automatically selected model\n"
			<< filename << "\n";

	}

	else if (cmd.target == Target::Load && file != cmd.options.end())
	{
		filename = file->second;

		if (auto it = std::find(files.begin(), files.end(), filename);
			it == files.end())
		{
			throw std::runtime_error("Invalid file for loading\n");
		}

	}



	return filename;

}



namespace Execute::Model
{
	void Save(const AIContext& ctx, const ParsedCommand& cmd)
	{

		std::string filename = getFileName(cmd);

		Persistence::save(ctx, filename);
	

	}



	void Load(AIContext& ctx, const ParsedCommand& cmd)
	{
		std::string filename = getFileName(cmd);

		Persistence::load(ctx, filename);
	}


}



[[maybe_unused]]
static std::string lowercaseString(std::string_view string)
{
	std::string lowerCase;

	std::for_each(string.begin(), string.end(),
		[&lowerCase](unsigned char c)
		{
			lowerCase += static_cast<char>(std::tolower(c));
		});

	return lowerCase;
}





static void applyGenerationConfig(AIContext& ctx, const ParsedCommand& cmd)
{
	if (cmd.options.contains("d-temp"))
	{
		double newTemp =
			OptionParser::getDoubleOption(
				cmd, "d-temp",
				ctx.getConfig().getTemp());


		//UI::WarningUI::temperature(newTemp);
		ctx.getConfig().setTemp(newTemp);



	}


	if (cmd.options.contains("d-max-length"))
	{
		//DOES ABSOLUTELY NOTHING YET
		int newMaxWords =
			OptionParser::getIntOption(
				cmd, "d-max-length",
				ctx.getConfig().getMaxWords());


		ctx.getConfig().setMaxWords(newMaxWords);

	}


	if (cmd.options.contains("d-model"))
	{
		std::string stringModel =
			OptionParser::getLowerCaseStringOption(
				cmd, "d-model",
				ModelTypeUtils::modelTypeToString(ctx.getConfig().getDefaultModel()));



		ctx.getConfig().setDefaultModel(
			ModelTypeUtils::stringToModelType(stringModel));

	}


	if (cmd.options.contains("d-ngram-size"))
	{
		size_t NGramSize =
			OptionParser::getSizeOption(
				cmd, "d-ngram-size",
				ctx.getConfig().getDefaultNGramSize());



		ctx.getConfig().setDefaultNGramSize(NGramSize);

	}

}


// SYNTAX
//[[nodiscard]]     // Warn if the return value is ignored.
//[[maybe_unused]]  // Suppress "unused" warnings.
//[[deprecated]]    // Warn when something is used.
//[[fallthrough]]   // Marks an intentional fallthrough in a switch.
//[[noreturn]]      // Function never returns (throws/exits forever).


// TO DO
// add warnings or some text for UI to display when user inputs trash that could technically go through
static void applyUIConfig(AIContext& ctx, const ParsedCommand& cmd)
{
	(void)ctx;

	if (cmd.options.contains("color-output"))
	{
		/*
		bool coloredOutput = 
			OptionParser::getBoolOption(
				cmd, "color-output",
				ctx.getConfig()./missing/
			)
		*/

	}



	if (cmd.options.contains("show-timings"))
	{
		/*
		bool shotTimings = 
			OptionParser::getBoolOption(
				cmd, "show-timings",
				ctx.getConfig(). / missing /
			)

		*/

	}


}


static void applyRuntimeConfig(AIContext& ctx, const ParsedCommand& cmd)
{
	if (cmd.options.contains("auto-save"))
	{
		bool autoSave =
			OptionParser::getBoolOption(
				cmd, "auto-save",
				ctx.getConfig().getAutoSave()
			);

		ctx.getConfig().setAutoSave(autoSave);

	}

	if (cmd.options.contains("auto-save-minutes"))
	{
		int saveMinutes =
			OptionParser::getIntOption(
				cmd, "auto-save-minutes",
				ctx.getConfig().getAutoSaveMinutes()
			);

		ctx.getConfig().setAutoSaveMinutes(saveMinutes);

	}
	

}



namespace Execute::Config
{
	void Apply(AIContext& ctx, const ParsedCommand& cmd)
	{
		if (cmd.options.empty())
			throw std::logic_error("At least 1 argument must exist\n");



		switch (cmd.target)
		{
			case Target::Generation:
			{
				
				applyGenerationConfig(ctx, cmd);

				break;
			}


			case Target::UI:
			{
				applyUIConfig(ctx, cmd);

				break;
			}




			case Target::Runtime:
			{
				
				applyRuntimeConfig(ctx, cmd);

				break;

			}


		default:
			throw std::invalid_argument("Invalid target for @config>\n");
		}




	}


}



static void debugModifier(AIContext& ctx, const ParsedCommand& cmd)
{
	
	if (cmd.options.contains("all"))
	{

		bool all = OptionParser::getBoolOption(cmd.options.at("all"));

		if (all)
			ctx.getDebugger().enableAll();
		else
			ctx.getDebugger().disableAll();
	}



	for (const auto& [name, value] : cmd.options)
	{
		auto it = debugMap.find(name);

		if (it == debugMap.end())
			continue;


		bool enabled = OptionParser::getBoolOption(value);

		
		if (enabled)
			ctx.getDebugger().enable(it->second);

		else
			ctx.getDebugger().disable(it->second);
	}


}

namespace Execute::Debug
{
	void Apply(AIContext& ctx, const ParsedCommand& cmd)
	{
		if (cmd.options.empty())
			throw std::logic_error("At least 1 argument must exist\n");

		
		//in case u add more later
		//who knows
		switch (cmd.target)
		{

			case Target::DebugFlags:
				debugModifier(ctx, cmd);
				break;

 
		default:
			throw std::invalid_argument("Invalid target for @config>\n");
		}


	}
}



static void printVersions()
{
	std::cout << "AMLE version: " << Version::AMLE << "\n";
	std::cout << "Save format: " << Version::SaveFormat << "\n";
}




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



	else if(stats.model == TrainModel::Legacy)
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
	void initiateTraining(const ParsedCommand& cmd, AIContext& ctx)
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


			if(!Compare::checkSimilarity(data.normalizedText, data.originalText, ctx.getTrainingInputs()))
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










namespace Execute
{
	void Executor(const ParsedCommand& cmd, AIContext& ctx)
	{

		switch (cmd.command)
		{
			case Command::Help:
			{
				//TO DO
				//UI::CommandUI::showAllCommands();
				std::cout << "W.I.P\n";

				break;
			}

			case Command::Clear:
			{
				system("cls");

				break;
			}

			case Command::Version:
			{
				printVersions();

				break;
			}





			case Command::Model:
			{
				switch (cmd.target)
				{
					case Target::Load:
						Execute::Model::Load(ctx, cmd);
						break;

					case Target::Save:
						Execute::Model::Save(ctx, cmd);
						break;

					case Target::Files:
					{
						auto files = Persistence::Files::getFiles();
						Persistence::Files::printAllFilesName(files);
						break;
					}

					default:
						throw std::logic_error("Invalid 'model' target.");



				}


		
				break;

			}



			case Command::Generate:
			{
				switch (cmd.target)
				{
					case Target::Text:
						Execute::Generate::Text(cmd, ctx);
						break;


					case Target::Word:
						Execute::Generate::Word(cmd, ctx);
						break;


					default:
						throw std::logic_error("Invalid 'generate' target.");
				}

				break;
			}



			case Command::Print:
			{
				switch (cmd.target)
				{
					
					case Target::Transitions:
					case Target::Probs:
					{
						choosePrinter(cmd, ctx);
						break;
					}

					case Target::Memory:
					{
						Execute::Print::Memory(cmd, ctx);
						break;
					}

					case Target::Vocab:
					{
						Execute::Print::Vocabulary(ctx);
						break;
					}

					default:
						throw std::logic_error("Invalid 'print' target.");
					}

				break;
			}



			case Command::Config:
			{
				Execute::Config::Apply(ctx, cmd);
				break;


			}



			case Command::Debug:
			{
				Execute::Debug::Apply(ctx, cmd);

				break;
			}



			case Command::Train:
			{
				switch (cmd.target)
				{
					case Target::Input:
						Execute::Training::initiateTraining(cmd, ctx);
						break;


					case Target::Self:
						//self train fucntion
						// maybe not the best ideea rn
						//Train::selfTrain(ctx, amount);
						break;


				default:
					throw std::logic_error("Invalid 'train' target.");


				}
			}


		}


	}


}




