#pragma once
#include <vector>
#include <string>

#include "AIContext.h"
#include "Memory.h"
#include "Vocabulary.h"
#include "Debug.h"
#include "Commands.h"
#include "TrainingInput.h"





namespace Execute::Clear
{	

	void cleaner(const ParsedCommand& cmd, AIContext& ctx);


	template<typename Map>
	void clearMap(Map& map);

	// TO DO
	std::string buildCleanerWarning(const ParsedCommand& cmd);
	bool checkCleaner(const ParsedCommand& cmd);



	namespace Model
	{
		void clearModel(AIContext& ctx);

	}



	namespace Memory
	{
		void clearMemory(AIMemory& memory);

		void clearTrainingInputs(std::vector<TrainingInput>& trainingInputs);

	}



	namespace Vocabulary
	{
		void clearVocabulary(VocabularyManager& vocabulary);
	}


	//::Debug* test = nullptr;
	/*
	namespace Debugger
	{
		void clearDebugger(::Debug& debugger);
	}
	*/

}



#include "CommandClearTemplates.h"