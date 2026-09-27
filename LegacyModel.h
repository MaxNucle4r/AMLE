#pragma once
#include "Types.h"
#include "Components.h"





class LegacyModel
{

private:

	const ComponentID component = ComponentID::LEGACY_MODEL;



	TextTypes::TransitionMap TextTransitions;
	TextTypes::ProbabilityMap TextProbs;

	CharTypes::TransitionMap CharTransitions;
	CharTypes::ProbabilityMap CharProbs;



public:

	const ComponentID& getID()
	{
		return component;
	}

	
	TextTypes::TransitionMap& getTextTransitions();
	TextTypes::ProbabilityMap& getTextProbs();


	const TextTypes::TransitionMap& getTextTransitions() const;
	const TextTypes::ProbabilityMap& getTextProbs() const;


	CharTypes::TransitionMap& getCharTransitions();
	CharTypes::ProbabilityMap& getCharProbs();


	const CharTypes::TransitionMap& getCharTransitions() const;
	const CharTypes::ProbabilityMap& getCharProbs() const;



	void clearTextTransitions();
	void clearTextProbs();

	void clearCharTransitions();
	void clearCharProbs();


	void clear();


	bool isEmpty() const;



	// TO DO
	LegacyModel& operator+= (const LegacyModel& other);

	LegacyModel& operator= (const LegacyModel& other);

	bool operator==(const LegacyModel& other) const;

};




// TO DO
LegacyModel operator+(LegacyModel left, const LegacyModel& right);