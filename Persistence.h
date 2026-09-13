#pragma once

#include <string>
#include "AIContext.h"
#include "Types.h"

#include "Memory.h"
#include "Vocabulary.h"



#pragma warning(push, 0)
#include <nlohmann/json.hpp>
#pragma warning(pop)
using json = nlohmann::ordered_json; 




namespace Persistence
{
	void save(const AIContext& ctx, const std::string& filename = "");

	void load(AIContext& ctx, const std::string& filename);


	void rebuildModels(AIContext& context);

	void saveMetadata(json& j, const AIContext& context);



	namespace Model
	{
		namespace Legacy
		{
			void saveLegacy(json& j,
				const TextTypes::TransitionMap& textTransitions,
				const CharTypes::TransitionMap& charTransitions);


			void loadLegacy(const json& j,
				TextTypes::TransitionMap& textTransitions,
				CharTypes::TransitionMap& charTransitions);
		}



		namespace NGram
		{
			void saveAllNgrams(json& j, 
				const std::map<size_t, NgramModel>& Ngrams);

			void loadAllNgrams(const json& j,
				std::map<size_t, NgramModel>& Ngrams);



			void saveNGram(json& j, 
				const TextTypes::NgramTransitionMap& textTransitions,
				const CharTypes::NgramTransitionMap& charTransitions);


			void loadNGram(const json& j, 
				TextTypes::NgramTransitionMap& textTransitions,
				CharTypes::NgramTransitionMap& charTransitions);
		}
		

	}



	namespace MemorySL
	{
		void saveMemory(json& j,
			const Memory& memory);


		void loadMemory(const json& j,
			Memory& memory);
	}



	namespace VocabularySL
	{
		void saveVocabulary(json& j, 
			const VocabularyManager& vocabManager);


		void loadVocabulary(const json& j,
			VocabularyManager& vocabManager);

	}



	namespace Detail
	{
		template<typename Map>
		void saveTransitions(json& j, const Map& map);

		template<typename Map>
		void loadTransitions(const json& j, Map& map);



		template<typename Context>
		json serializeContext(const Context& context);

		template<typename Context>
		Context deserializeContext(const json& jsonContext);


	}


}

#include "PersistenceTemplates.h"