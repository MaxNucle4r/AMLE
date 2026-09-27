#pragma once
#include <map>

#include "Vocabulary.h"
#include "Memory.h"
#include "Debug.h"
#include "Configuration.h"
#include "TrainingInput.h"
#include "LegacyModel.h"
#include "NGram.h"

#include "Components.h"





class AIContext
{

private:

	const ComponentID component = ComponentID::AI_CONTEXT;


	// models
	LegacyModel legacy;

	std::map<size_t, NgramModel> NGrams;



	VocabularyManager vocabulary;


	AIMemory memory;


	Debug debugger;


	Configuration config;


	//per session only
	std::vector<TrainingInput> trainingInputs;



public:

	AIContext();





	const ComponentID& getID() const
	{
		return component;
	}



	LegacyModel& getLegacyModel()
	{
		return legacy;
	}

	const LegacyModel& getLegacyModel() const 
	{
		return legacy;
	}





	const std::map<size_t, NgramModel>& getAllNGrams() const
	{
		return NGrams;
	}

	std::map<size_t, NgramModel>& getAllNGrams()
	{
		return NGrams;
	}


	NgramModel& getNGram(size_t i)
	{
		return NGrams.at(i);
	}

	const NgramModel& getNGram(size_t i) const
	{
		return NGrams.at(i);
	}





	VocabularyManager& getVocabulary()
	{
		return vocabulary;
	}


	const VocabularyManager& getVocabulary() const
	{
		return vocabulary;
	}





	AIMemory& getMemory()
	{
		return memory;
	}


	const AIMemory& getMemory() const
	{
		return memory;
	}





	Debug& getDebugger()
	{
		return debugger;
	}


	const Debug& getDebugger() const
	{
		return debugger;
	}





	Configuration& getConfig()
	{
		return config;
	}


	const Configuration& getConfig() const
	{
		return config;
	}


	


	std::vector<TrainingInput>& getTrainingInputs()
	{
		return trainingInputs;
	}


	const std::vector<TrainingInput>& getTrainingInputs() const
	{
		return trainingInputs;
	}





	void clearMemory();
	void clearTrainingInputs();

	void clearVocabulary();

	void clearDebugger();


	void clearLegacy();

	void clearAllNGrams();
	void clearNGram(size_t size);


	void clearModel();


	bool areNgramsEmpty() const;
	bool isLegacyEmpty() const;
	bool isEmpty() const;





	AIContext& operator+= (const AIContext& other);

	AIContext& operator= (const AIContext& other);

	// TO DO
	bool operator==(const AIContext& other) const;


};





AIContext operator+(AIContext left, const AIContext& right);

