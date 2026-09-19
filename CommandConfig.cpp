#include "CommandConfig.h"
#include "AIContext.h"
#include "Option Parser.h"
#include "ModelTypeConversion.h"
#include "UI.h"
#include "Commands.h"






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