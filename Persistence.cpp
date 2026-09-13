#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>



#include "Persistence.h"
#include "FileSystem.h"
#include "TimeStamp.h"
#include "Model.h"
#include "Version.h"
#include "Debug.h"





namespace
{

	template <typename Map>
	size_t countTransitions(const Map& map)
	{
		size_t transitions = 0;

		for (const auto& [context, nextMap] : map)
			transitions += nextMap.size();

		return transitions;

	}

	template <typename Map>
	size_t countTransitionOccurrences(const Map& map)
	{
		size_t individualTransitions = 0;

		for (const auto& [context, nextMap] : map)
		{
			for (const auto& [next, count] : nextMap)
			{
				individualTransitions += count;
			}
		}

		return individualTransitions;
	}


}
	

void Persistence::saveMetadata(json& j, const AIContext& context)
{
	j["metadata"]["formatVersion"] = Version::SaveFormat;
	j["metadata"]["AMLEVersion"] = Version::AMLE;


	j["metadata"]["created"] = j["metadata"]["lastUsed"] = Time::getTimestamp();



	auto& legacyStats = j["metadata"]["stats"]["legacy"];


	legacyStats["text"]["contexts"] =
		context.getTextTransitions().size();

	legacyStats["text"]["transitions"] =
		countTransitions(context.getTextTransitions());

	legacyStats["text"]["transitionOccurrences"] =
		countTransitionOccurrences(context.getTextTransitions());



	legacyStats["char"]["contexts"] =
		context.getCharTransitions().size();

	legacyStats["char"]["transitions"] =
		countTransitions(context.getCharTransitions());

	legacyStats["char"]["transitionOccurrences"] =
		countTransitionOccurrences(context.getCharTransitions());





	// map < size_t, NGramModel >
	for (const auto& [size, model] : context.getAllNGrams())
	{
		auto& ngramStats =
			j["metadata"]["stats"]["ngram"]["size " + std::to_string(size)];
		


		ngramStats["text"]["contexts"] =
			model.getTextTransitions().size();

		ngramStats["text"]["transitions"] =
			countTransitions(model.getTextTransitions());

		ngramStats["text"]["transitionOccurrences"] =
			countTransitionOccurrences(model.getTextTransitions());



		ngramStats["char"]["contexts"] =
			model.getCharTransitions().size();

		ngramStats["char"]["transitions"] =
			countTransitions(model.getCharTransitions());

		ngramStats["char"]["transitionOccurrences"] =
			countTransitionOccurrences(model.getCharTransitions());


	}


}





namespace Persistence::Model::Legacy
{

	void saveLegacy(json & j,
			const TextTypes::TransitionMap & textTransitions,
			const CharTypes::TransitionMap & charTransitions)
	{

		Persistence::Detail::saveTransitions(j["textTransitions"], textTransitions);


		Persistence::Detail::saveTransitions(j["charTransitions"], charTransitions);


	}



	void loadLegacy(const json& j,
		TextTypes::TransitionMap& textTransitions,
		CharTypes::TransitionMap& charTransitions)
	{
		// *** maps are cleared inside the functions ***

		Persistence::Detail::loadTransitions(j["textTransitions"], textTransitions);
		Persistence::Detail::loadTransitions(j["charTransitions"], charTransitions);

	}


}



namespace Persistence::Model::NGram
{

	void saveNGram(json& j,
		const TextTypes::NgramTransitionMap& textTransitions,
		const CharTypes::NgramTransitionMap& charTransitions)
	{

		Persistence::Detail::saveTransitions(j["textTransitions"], textTransitions);

		Persistence::Detail::saveTransitions(j["charTransitions"], charTransitions);

	}



	void loadNGram(const json& j,
		TextTypes::NgramTransitionMap& textTransitions,
		CharTypes::NgramTransitionMap& charTransitions)
	{
		// *** maps are cleared inside the functions ***

		Persistence::Detail::loadTransitions(j["textTransitions"], textTransitions);

		Persistence::Detail::loadTransitions(j["charTransitions"], charTransitions);

	}

}





namespace Persistence::Model::NGram
{
	void saveAllNgrams(json& j, const std::map<size_t, NgramModel>& Ngrams)
	{

		for (const auto& [size, model] : Ngrams)
		{
			Persistence::Model::NGram::saveNGram(
				j["ngram"]["size " + std::to_string(size)],
				model.getTextTransitions(),
				model.getCharTransitions()
			);
		}

	}


	void loadAllNgrams(const json& j, std::map<size_t, NgramModel>& Ngrams)
	{
		Ngrams.clear();

		for (const auto& [key, data] : j["ngram"].items())
		{

			size_t size = std::stoul(
				key.substr(std::string("size ").length())
			);


			Ngrams.emplace(size, NgramModel(size));

			auto& model = Ngrams.at(size);


			Persistence::Model::NGram::loadNGram(
				data,
				model.getTextTransitions(),
				model.getCharTransitions()
			);
		}
	}


}




void Persistence::save(const AIContext& ctx, const std::string& filename)
{
	
	Files::ensureModelsDirectory();


	const auto path = Files::getSafeModelPath(filename);

	std::cout << "FILE WILL BE SAVED AS: " << filename << "\n\n";



	if (ctx.getDebugger().enabled(DebugFlags::Persistence))
	{
		std::cout << "FILE WILL BE: " << path << "\n";
	}


	json j;

	saveMetadata(j, ctx);

	if (ctx.getDebugger().enabled(DebugFlags::Persistence))
	{
		std::cout << "\n=== AFTER METADATA ===\n";
		std::cout << "metadata: "
			<< j.contains("metadata") << '\n';
		std::cout << "legacy: "
			<< j.contains("legacy") << '\n';
		std::cout << "ngram: "
			<< j.contains("ngram") << '\n';
	}

	



	Model::Legacy::saveLegacy(
		j["legacy"],
		ctx.getTextTransitions(),
		ctx.getCharTransitions()
	);


	if (ctx.getDebugger().enabled(DebugFlags::Persistence))
	{
		std::cout << "\n=== AFTER LEGACY ===\n";
		std::cout << "metadata: "
			<< j.contains("metadata") << '\n';
		std::cout << "legacy: "
			<< j.contains("legacy") << '\n';
		std::cout << "ngram: "
			<< j.contains("ngram") << '\n';
	}

	


	// it does j["ngram"]["size"] inside the funciton
	Model::NGram::saveAllNgrams(j, ctx.getAllNGrams());


	if (ctx.getDebugger().enabled(DebugFlags::Persistence))
	{
		std::cout << "\n=== AFTER NGRAM ===\n";
		std::cout << "metadata: "
			<< j.contains("metadata") << '\n';
		std::cout << "legacy: "
			<< j.contains("legacy") << '\n';
		std::cout << "ngram: "
			<< j.contains("ngram") << '\n';
	}


	
	if (ctx.getDebugger().enabled(DebugFlags::Persistence))
	{
		std::cout << "formatVersion: "
			<< j["metadata"]["formatVersion"] << '\n';
	}




	//Vocab
	//Memory






	json output;

	output["metadata"] = j["metadata"];
	output["legacy"] = j["legacy"];
	output["ngram"] = j["ngram"];


	Files::JSON::writeFile(output, path);


}



void Persistence::rebuildModels(AIContext& context)
{
	 
	context.getCharProbs() =
		::Model::Char::makeProbabilities(context.getCharTransitions());

	context.getTextProbs() =
		::Model::Text::makeProbabilities(context.getTextTransitions());


	for (auto& [size, model] : context.getAllNGrams())
	{
		model.getCharProbabilities() =
			::Model::Char::makeProbabilitiesNGram(model.getCharTransitions());

		model.getTextProbabilities() =
			::Model::Text::makeProbabilitiesNGram(model.getTextTransitions());

	}


	

}


void Persistence::load(AIContext& ctx, const std::string& filename)
{
	const auto path = Files::getSafeModelPath(filename);


	json j = Files::JSON::readFile(path);

	// version check

	Model::Legacy::loadLegacy(
		j["legacy"],
		ctx.getTextTransitions(),
		ctx.getCharTransitions()
	);

	Model::NGram::loadAllNgrams(
		j,
		ctx.getAllNGrams()
	);

	// Memory
	// Vocabulary

	rebuildModels(ctx);

}