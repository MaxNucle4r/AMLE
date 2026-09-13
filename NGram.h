#pragma once
#include "AMLE_ASSERT.h"


#include "Types.h"





void NGramContextSizeRecommendation(size_t size);


class NgramModel
{
private:

	const size_t contextSize;


	TextTypes::NgramTransitionMap textTransitions;
	CharTypes::NgramTransitionMap charTransitions;


	TextTypes::NgramProbabilityMap textProbabilities;
	CharTypes::NgramProbabilityMap charProbabilities;


public:

	static constexpr size_t MIN_CONTEXT_SIZE = 2;
	static constexpr size_t MAX_CONTEXT_SIZE = 7;

	static constexpr size_t RECOMMENDED_MIN_CONTEXT_SIZE = 2;
	static constexpr size_t RECOMMENDED_MAX_CONTEXT_SIZE = 5;


	// funciton belongs to the class itself
	// not individual object
	static void validateContextSize(size_t size);



	//explicit means no automatic conversions
	/*
	if u have something like
	void doSomething(NgramModel model)
	{
	}
	then do doSomething(4);
	4 gets automatically converted into an NgramModel

	explicit controls whether the constructor can be used for automatic type conversion
	
	*/
	explicit NgramModel(size_t size)
		: contextSize(size)
	{
		validateContextSize(size);


		AMLE_ASSERT(contextSize >= MIN_CONTEXT_SIZE);
		AMLE_ASSERT(contextSize <= MAX_CONTEXT_SIZE);
		
	}


	NgramModel() = delete;




	size_t getContextSize() const;



	TextTypes::NgramTransitionMap& getTextTransitions();
	const TextTypes::NgramTransitionMap& getTextTransitions() const;


	CharTypes::NgramTransitionMap& getCharTransitions();
	const CharTypes::NgramTransitionMap& getCharTransitions() const;



	TextTypes::NgramProbabilityMap& getTextProbabilities();
	const TextTypes::NgramProbabilityMap& getTextProbabilities() const;

	CharTypes::NgramProbabilityMap& getCharProbabilities();
	const CharTypes::NgramProbabilityMap& getCharProbabilities() const;





	NgramModel& operator=(const NgramModel& other);

	NgramModel& operator+=(const NgramModel& other);




};