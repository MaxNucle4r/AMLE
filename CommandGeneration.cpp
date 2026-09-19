#include "CommandGeneration.h"
#include "Option Parser.h"
#include "Score.h"
#include "ModelTypeConversion.h"





namespace Generation::GenerationHelpers
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



	size_t pickModelTypeSize(const ParsedCommand& cmd, const size_t defaultSize)
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





		size_t modelSize = Generation::GenerationHelpers::pickModelTypeSize(cmd, ctx.getConfig().getDefaultNGramSize());


		ModelType model = Generation::GenerationHelpers::pickModelType(ctx.getConfig().getDefaultModel(), cmd);


		seed = Generation::GenerationHelpers::pickGenerationSeed(cmd, ctx.getDebugger());



		GenerationType gentype = GenerationType::NONE;
		EndingType endtype = EndingType::NONE;




		std::string reply;

		if (model == ModelType::Legacy)
			reply = Generation::Text(ctx.getTextProbs(), temperature, endtype, gentype, seed, ctx.getDebugger());

		else if (model == ModelType::NGram)
			reply = Generation::Text(ctx.getNGram(modelSize).getTextProbabilities(), temperature, endtype, gentype, seed, ctx.getDebugger(), modelSize);



		Generation::GenerationHelpers::handleGenerationDebug(ctx.getDebugger(), reply, endtype, ctx.getVocabulary());



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