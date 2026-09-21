#include "CommandPrint.h"
#include "Option Parser.h"
#include "ModelTypeConversion.h"
#include "ValidationTemplatesh.h"
#include "UI.h"





namespace Execute::Print
{
	void TextTransitions(const TextTypes::TransitionMap& transitions)
	{
		UI::Legacy::printTransitions(transitions);
	}

	void TextProbabilities(const TextTypes::ProbabilityMap& probs)
	{
		UI::Legacy::printProbabilities(probs);
	}



	void NGramTextTransitions(const TextTypes::NgramTransitionMap& transitions)
	{
		UI::NGram::printTransitions(transitions);
	}

	void NGramTextProbabilities(const TextTypes::NgramProbabilityMap& probs)
	{
		UI::NGram::printProbabilities(probs);
	}





	void CharTransitions(const CharTypes::TransitionMap& transitions)
	{
		UI::Legacy::printTransitions(transitions);
	}

	void CharProbabilities(const CharTypes::ProbabilityMap& probs)
	{
		UI::Legacy::printProbabilities(probs);
	}



	void NGramCharTransitions(const CharTypes::NgramTransitionMap& transitions)
	{
		UI::NGram::printTransitions(transitions);
	}

	void NGramCharProbabilities(const CharTypes::NgramProbabilityMap& probs)
	{
		UI::NGram::printProbabilities(probs);
	}




	
	// it can accept both -10 and 10 as values
	// 10 = last 10
	// -10 = first 10
	void Memory(const ParsedCommand& cmd, const AIMemory& memory)
	{
		int size = 0;

		if (cmd.options.contains("size"))
		{
			size = OptionParser::getIntOption(cmd, "size");
		}

		else
		{
			UI::Memory::printMemory(memory.getAll());

			return;
		}



		size_t requested = static_cast<size_t>(std::abs(size));
		

		size_t amount = Validation::clampWithWarning("Size", requested, 
			static_cast<size_t>(0), memory.size());



		if (size > 0)
			UI::Memory::printMemory(memory.getLast(amount));

		else if (size < 0)
			UI::Memory::printMemory(memory.getFirst(amount));

		else if (size == 0)
			throw std::invalid_argument("Size cannot be 0");


	}



	void Vocabulary(const VocabularyManager& vocab)
	{
		UI::Vocab::Vocabulary(vocab);
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

		modelSize = Validation::clampWithWarning("ngram-size", 
			modelSize, 
			NgramModel::MIN_CONTEXT_SIZE, NgramModel::MAX_CONTEXT_SIZE);

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
			Execute::Print::TextTransitions(ctx.getTextTransitions());

		if (options.character)
			Execute::Print::CharTransitions(ctx.getCharTransitions());
	}

	else if (cmd.target == Target::Probs)
	{
		if (options.text)
			Execute::Print::TextProbabilities(ctx.getTextProbs());

		if (options.character)
			Execute::Print::CharProbabilities(ctx.getCharProbs());
	}

}


static void handleNGramModelPrinting(const ParsedCommand& cmd, const AIContext& ctx, const size_t modelSize)
{
	PrintOptions options = getPrintOptions(cmd);



	if (cmd.target == Target::Transitions)
	{
		if (options.text)
			Execute::Print::NGramTextTransitions(ctx.getNGram(modelSize).getTextTransitions());

		if (options.character)
			Execute::Print::NGramCharTransitions(ctx.getNGram(modelSize).getCharTransitions());
	}


	else if (cmd.target == Target::Probs)
	{
		if (options.text)
			Execute::Print::NGramTextProbabilities(ctx.getNGram(modelSize).getTextProbabilities());

		if (options.character)
			Execute::Print::NGramCharProbabilities(ctx.getNGram(modelSize).getCharProbabilities());
	}



}


static bool handleGenerincPrinting(const ParsedCommand& cmd,
	const AIContext& ctx)
{
	if (cmd.target == Target::Transitions)
	{
		if (cmd.options.empty())
		{
			Execute::Print::TextTransitions(ctx.getTextTransitions());

			Execute::Print::NGramTextTransitions(ctx.getNGram(ctx.getConfig().getDefaultNGramSize()).getTextTransitions());


			Execute::Print::CharTransitions(ctx.getCharTransitions());

			Execute::Print::NGramCharTransitions(ctx.getNGram(ctx.getConfig().getDefaultNGramSize()).getCharTransitions());

			return true;
		}
	}

	if (cmd.target == Target::Probs)
	{
		if (cmd.options.empty())
		{
			//NGRAM PROBS PRINTER NOT DONE

			Execute::Print::TextProbabilities(ctx.getTextProbs());

			Execute::Print::CharProbabilities(ctx.getCharProbs());

			return true;
		}
	}

	return false;

}



void choosePrinter(const ParsedCommand& cmd,
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





