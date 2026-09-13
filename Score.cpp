#include <algorithm>
#include <sstream>
#include <iostream>

#include "Score.h"





namespace Scoring::Word
{
	float score(const std::string& word, const VocabularyManager& vm, const Debug& debugger)
	{
        
            //gonna be the hardest one to balance
            //or it will somehow be good to go from the first try
            //or it might break this ... thing? yeah... i can call it a thing

            float score = 0.0f;

            if (word.size() >= 2 && word.size() <= 10) score += 2.0f;

            else if (word.size() <= 15) score += 1.0f;

            else score -= 2.0f;

            if (debugger.enabled(DebugFlags::Scoring))
                std::cout << "Current score: " << score << "\n";



            int vowelCount = 0;

            for (const char c : word)
            {
                char lower = static_cast<char>(std::tolower((unsigned char)c));

                if (lower == 'a' || lower == 'e' ||
                    lower == 'i' || lower == 'o' ||
                    lower == 'u')
                {
                    vowelCount++;
                }
            }

            if (vowelCount > 0) score += 2.0f;

            else score -= 3.0f;



            int repeated = 0;

            for (size_t i = 1; i < word.size(); ++i)
            {
                if (word[i] == word[i - 1]) repeated++;
            }

            if (repeated <= 2) score += 1.0f;

            else score -= 2.0f;



            for (const auto& [knownWord, stats] : vm.getAll())
            {
                if (knownWord.find(word) != std::string::npos ||
                    word.find(knownWord) != std::string::npos)
                {
                    score += 3.0f;
                    break;
                }
            }


            return score;

	}

}





namespace Scoring::Text
{
    float score(const std::string& text, const VocabularyManager& vm, bool ignoreLenghtPenalty, const Debug& debugger)
    {
        float score = 0.0f;

        std::vector<std::string> words;

        std::stringstream ss(text);
        std::string word;

        while (ss >> word)
        {
            words.push_back(word);

            score += Scoring::Word::score(word, vm, debugger);
        }

        if (words.empty()) return 0.0f;



        if (words.size() == 1) score -= 2.0f;

        if (!ignoreLenghtPenalty)
        {
            if (words.size() >= 3 && words.size() <= 15)
                score += 3.0f;


            if (words.size() > 25)
                score -= (words.size() - 25) * 0.25f;
        }


        for (size_t i = 1; i < words.size(); ++i)
        {
            if (words[i] == words[i - 1])
                score -= 2.5f;
        }



        std::unordered_map<std::string, int> seen;

        for (const auto& w : words) seen[w]++;

        for (const auto& [w, count] : seen)
        {
            if (count > 3)
            {
                // punish overused words
                score -= (count - 3) * 1.5f;
            }
        }



        int knownWords = 0;

        for (const auto& w : words)
        {
            if (vm.exists(w))
                knownWords++;
        }

        float knownRatio = static_cast<float>(knownWords) / words.size();

        score += knownRatio * 5.0f;


        score /= words.size();

        return score;
    }

}




