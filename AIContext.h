#pragma once
#include <map>

#include "Types.h"
#include "Vocabulary.h"
#include "Memory.h"
#include "Debug.h"
#include "Configuration.h"
#include "TrainingInput.h"
#include "NGram.h"





class AIContext
{

private:


	// the default legacy model
	TextTypes::TransitionMap TextTransitions;
	TextTypes::ProbabilityMap TextProbs;

	CharTypes::TransitionMap CharTransitions;
	CharTypes::ProbabilityMap CharProbs;



	// generic NGram models
	std::map<size_t, NgramModel> NGrams;



	VocabularyManager vocabulary;


	AIMemory memory;


	Debug debugger;


	Configuration config;


	//per session only
	std::vector<TrainingInput> trainingInputs;



public:

	AIContext();





	TextTypes::TransitionMap& getTextTransitions() 
	{
		return TextTransitions;
	}

	TextTypes::ProbabilityMap& getTextProbs() 
	{
		return TextProbs;
	}


	const TextTypes::TransitionMap& getTextTransitions() const
	{
		return TextTransitions;
	}
	
	const TextTypes::ProbabilityMap& getTextProbs() const
	{
		return TextProbs;
	}





	CharTypes::TransitionMap& getCharTransitions() 
	{
		return CharTransitions;
	}

	CharTypes::ProbabilityMap& getCharProbs()
	{
		return CharProbs;
	}


	const CharTypes::TransitionMap& getCharTransitions() const
	{
		return CharTransitions;
	}

	const CharTypes::ProbabilityMap& getCharProbs() const
	{
		return CharProbs;
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





	AIContext& operator+= (const AIContext& other);

	AIContext& operator= (const AIContext& other);

	// TO DO
	bool operator==(const AIContext& other) const;


};





AIContext operator+(AIContext left, const AIContext& right);

