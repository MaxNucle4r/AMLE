
#include "Model.h"
#include "AIContext.h"





// by default only create 7
// for now at least...
// use map so they are ordered nicely
// so that NGrams[3] gives the 3Gram
// DECIDE LATER WHETHER U WANT SIZE 1 OR NOT.
// as of 01.08.26 --- 12.45 U REMOVED IT
AIContext::AIContext()
{
    for (size_t size = NgramModel::MIN_CONTEXT_SIZE; size <= NgramModel::MAX_CONTEXT_SIZE; ++size)
    {
        NGrams.emplace(size, NgramModel{ size });
    }
}





AIContext& AIContext::operator=(const AIContext& other)
{
    //legacy model
    TextTransitions = other.getTextTransitions();
    TextProbs = other.getTextProbs();

    CharTransitions = other.getCharTransitions();
    CharProbs = other.getCharProbs();



    for (auto& [size, ngram] : NGrams)
    {
        ngram = other.getNGram(size);
    }


    vocabulary = other.getVocabulary();


    //do not touch these yet:
    // debugger
    // config


    trainingInputs = other.getTrainingInputs();
    
    

    return *this;

}




namespace
{
    void mergeTextTransitions(
        TextTypes::TransitionMap& destination,
        const TextTypes::TransitionMap& source)
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
        CharTypes::TransitionMap& destination,
        const CharTypes::TransitionMap& source)
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


AIContext& AIContext::operator+= (const AIContext& other)
{
    mergeTextTransitions(TextTransitions, other.getTextTransitions());
    mergeCharTransitions(CharTransitions, other.getCharTransitions());

    TextProbs = Model::Text::makeProbabilities(TextTransitions);
    CharProbs = Model::Char::makeProbabilities(CharTransitions);





    for (const auto& [size, ngram] : other.getAllNGrams())
    {
        //1st size = key
        //2nd size = constructor argument
        auto [it, inserted] = NGrams.try_emplace(size, size);

        it->second += ngram;
    }


    for (auto& [size, ngram] : NGrams)
    {
        ngram.getTextProbabilities() = Model::Text::makeProbabilitiesNGram(ngram.getTextTransitions());
        ngram.getCharProbabilities() = Model::Char::makeProbabilitiesNGram(ngram.getCharTransitions());
    }





    trainingInputs.insert(
        trainingInputs.end(),
        other.getTrainingInputs().begin(),
        other.getTrainingInputs().end()
    );


    memory += other.getMemory();

    vocabulary += other.getVocabulary();



    return *this;


}





void AIContext::clearMemory()
{
    memory.clear();

}

void AIContext::clearTrainingInputs()
{
    trainingInputs.clear();
}



void AIContext::clearVocabulary()
{
    vocabulary.clear();
}



void AIContext::clearDebugger()
{
    debugger.disableAll();
}



void AIContext::clearLegacyTextTransitions()
{
    TextTransitions.clear();
}

void AIContext::clearLegacyTextProbs()
{
    TextProbs.clear();
}

void AIContext::clearLegacyCharTransitions()
{
    AIContext::CharTransitions.clear();
}

void AIContext::clearLegacyCharProbs()
{
    CharProbs.clear();
}


void AIContext::clearAllNGrams()
{
    NGrams.clear();
}

void AIContext::clearNGram(size_t size)
{
    if (!(size < NgramModel::MAX_CONTEXT_SIZE &&
        size > NgramModel::MIN_CONTEXT_SIZE))
    {
        throw std::invalid_argument("Invalid Option For Size");
    }

    NGrams.at(size).clear();
}



void AIContext::clearModel()
{
   
    clearLegacyTextTransitions();
    clearLegacyTextProbs();

    clearLegacyCharTransitions();
    clearLegacyCharProbs();

    clearAllNGrams();

    clearMemory();
    clearTrainingInputs();
    clearVocabulary();

}

