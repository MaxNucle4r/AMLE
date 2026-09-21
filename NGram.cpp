#include <stdexcept>

#include "NGram.h"
#include "UI.h"
#include "Model.h"





void NGramContextSizeRecommendation(size_t size)
{
    if (size < 2)
    {
        std::cout
            << "NGram context size " << size
            << " may produce poor results.\n";
    }

    else if (size > 5)
    {
        std::cout
            << "NGram context size " << size
            << " requires significantly more training data.\n";
    }
}





void NgramModel::validateContextSize(size_t size)
{
    if (size < MIN_CONTEXT_SIZE)
    {
        throw std::invalid_argument(
            "Context size " + std::to_string(size) +
            " is too small. Minimum is " +
            std::to_string(MIN_CONTEXT_SIZE)
        );
    }

    if (size > MAX_CONTEXT_SIZE)
    {
        throw std::invalid_argument(
            "Context size " + std::to_string(size) +
            " is too big. Maximum is " +
            std::to_string(MAX_CONTEXT_SIZE)
        );
    }
}



size_t NgramModel::getContextSize() const
{
	return contextSize;
}


void NgramModel::clear()
{
    textTransitions.clear();
    textProbabilities.clear();

    charTransitions.clear();
    charProbabilities.clear();
}



TextTypes::NgramTransitionMap& NgramModel::getTextTransitions()
{
	return textTransitions;
}

CharTypes::NgramTransitionMap& NgramModel::getCharTransitions()
{
	return charTransitions;
}



TextTypes::NgramProbabilityMap& NgramModel::getTextProbabilities()
{
	return textProbabilities;
}

CharTypes::NgramProbabilityMap& NgramModel::getCharProbabilities()
{
	return charProbabilities;
}



const TextTypes::NgramTransitionMap& NgramModel::getTextTransitions() const
{
	return textTransitions;
}

const CharTypes::NgramTransitionMap& NgramModel::getCharTransitions() const
{
	return charTransitions;
}


const TextTypes::NgramProbabilityMap& NgramModel::getTextProbabilities() const
{
	return textProbabilities;
}

const CharTypes::NgramProbabilityMap& NgramModel::getCharProbabilities() const 
{
	return charProbabilities;
}





NgramModel& NgramModel::operator=(const NgramModel& other)
{

    if (contextSize != other.contextSize)
    {
        throw std::invalid_argument(
            "Cannot assign models with different context sizes."
        );
    }



    textTransitions = other.textTransitions;
    charTransitions = other.charTransitions;

    textProbabilities = other.textProbabilities;
    charProbabilities = other.charProbabilities;



    return *this;

}





namespace
{
    void mergeTextTransitions(
        TextTypes::NgramTransitionMap& destination,
        const TextTypes::NgramTransitionMap& source)
    {
        for (const auto& [context, nextWords] : source)
        {
            auto& destinationNextWords = destination[context];

            for (const auto& [word, count] : nextWords)
            {
                destinationNextWords[word] += count;
            }
        }
    }



    void mergeCharTransitions(
        CharTypes::NgramTransitionMap& destination,
        const CharTypes::NgramTransitionMap& source)
    {
        for (const auto& [context, nextChars] : source)
        {
            auto& destinationNextChars = destination[context];

            for (const auto& [character, count] : nextChars)
            {
                destinationNextChars[character] += count;
            }
        }
    }


}



NgramModel& NgramModel::operator+=(const NgramModel& other)
{
    mergeTextTransitions(
        textTransitions,
        other.getTextTransitions()
    );

    mergeCharTransitions(
        charTransitions,
        other.getCharTransitions()
    );

    textProbabilities = Model::Text::makeProbabilitiesNGram(textTransitions);
    charProbabilities = Model::Char::makeProbabilitiesNGram(charTransitions);

    return *this;
}