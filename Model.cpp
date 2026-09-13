#include "Model.h"




namespace Model::Text
{
    TextTypes::ProbabilityMap makeProbabilities(const TextTypes::TransitionMap& transitions)
    {
        TextTypes::ProbabilityMap probs;

        for (const auto& [context, nextMap] : transitions)
        {
            int total = 0;

            for (const auto& [word, count] : nextMap)
                total += count;

            if (total == 0) continue;

            for (const auto& [word, count] : nextMap)
                probs[context][word] = static_cast<double>(count) / total;
        }

        return probs;

    }

}



namespace Model::Char
{
    CharTypes::ProbabilityMap makeProbabilities(const CharTypes::TransitionMap& charTransitions)
    {
        CharTypes::ProbabilityMap charProbs;

        for (const auto& [chars, nextCharMap] : charTransitions)
        {
            int total = 0;

            for (const auto& [c, count] : nextCharMap)
                total += count;

            if (total == 0) continue;

            for (const auto& [c, count] : nextCharMap)
                charProbs[chars][c] = static_cast<double>(count) / total;


        }

        return charProbs;

    }

}





namespace Model::Text
{
    TextTypes::NgramProbabilityMap makeProbabilitiesNGram(const TextTypes::NgramTransitionMap& transitions)
    {

        TextTypes::NgramProbabilityMap probabilities;


        for (const auto& [context, nextMap] : transitions)
        {
            int total = 0;

            for (const auto& [word, count] : nextMap)
            {
                total += count;

            }


            if (total == 0)
                continue;


            for (const auto& [word, count] : nextMap)
            {
                probabilities[context][word] = static_cast<double>(count) / total;
            }


        }


        return probabilities;


    }


}



namespace Model::Char
{
    CharTypes::NgramProbabilityMap makeProbabilitiesNGram(const CharTypes::NgramTransitionMap& transitions)
    {

        CharTypes::NgramProbabilityMap probs;


        for (const auto& [context, nextMap] : transitions)
        {
            int total = 0;

            for (const auto& [character, count] : nextMap)
            {
                total += count;
            }


            if (total == 0)
                
                continue;

            for (const auto& [character, count] : nextMap)
            {
                probs[context][character] =
                    static_cast<double>(count) / total;
            }

        }


        return probs;
    }

}

