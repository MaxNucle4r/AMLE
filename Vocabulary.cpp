#include "Vocabulary.h"

#include <fstream>
#include <iostream>





const std::unordered_map<std::string, WordStats>& VocabularyManager::getAll() const
{
    return vocab;
}

std::unordered_map<std::string, WordStats>& VocabularyManager::getAll()
{
    return vocab;
}





void VocabularyManager::addInputWord(const std::string& word)
{
    WordStats& stats = vocab[word];

    stats.inputCount++;


    if (stats.source == WordSource::Generated)
        stats.source = WordSource::Both;

    else
        stats.source = WordSource::Input;
}


void VocabularyManager::addInputWord(const WordData& word)
{
    WordStats& stats = vocab[word.letters];

    stats.inputCount++;


    if (word.wasCaps)
        stats.uppercaseSeen++;

    else
        stats.lowercaseSeen++;


    if (stats.source == WordSource::Generated)
        stats.source = WordSource::Both;

    else
        stats.source = WordSource::Input;


}



void VocabularyManager::addGeneratedWord(const std::string& word)
{
    WordStats& stats = vocab[word];

    stats.generatedCount++;


    if (stats.source == WordSource::Input)
        stats.source = WordSource::Both;

    else
        stats.source = WordSource::Generated;
}



void VocabularyManager::setWord(const std::string& word, const WordStats& stats)
{
    vocab[word] = stats;
}





bool VocabularyManager::exists(const std::string& word) const
{
    return vocab.find(word) != vocab.end();
}



const WordStats& VocabularyManager::getWordStats(const std::string& word) const
{
    return vocab.at(word);
}





void VocabularyManager::displayVocabulary() const
{
    for (const auto& [word, stats] : vocab)
    {
        std::cout << word << ": \n"
            << "--> I.C: " << stats.inputCount << "\n"
            << "--> G.C: " << stats.generatedCount << "\n";

        std::cout << "\n";

    }

}





size_t VocabularyManager::size() const
{
    return vocab.size();
}





namespace
{
    void mergeVocabularies(VocabularyManager& destination,
        const VocabularyManager& source)
    {
        for (const auto& [word, stats] : source.getAll())
        {
            auto& wordStats = destination.getAll()[word];

            wordStats.inputCount += stats.inputCount;

            wordStats.generatedCount += stats.generatedCount;



            if (wordStats.inputCount > 0 && wordStats.generatedCount > 0)
                wordStats.source = WordSource::Both;

            else if (wordStats.inputCount > 0)
                wordStats.source = WordSource::Input;

            else if (wordStats.generatedCount > 0)
                wordStats.source = WordSource::Generated;

            else
                //prob a  bug
                wordStats.source = WordSource::NONE;



            wordStats.lowercaseSeen += stats.lowercaseSeen;
            wordStats.uppercaseSeen += stats.uppercaseSeen;


        }
    }
}

VocabularyManager& VocabularyManager::operator+= (const VocabularyManager& other)
{
    mergeVocabularies(*this, other);


    return *this;

}


