#pragma once

#include <unordered_map>
#include <string>

#include "Text Processing.h"





enum class WordSource { NONE, Input, Generated, Both };


struct WordStats
{
    int inputCount = 0;
    int generatedCount = 0;

    int lowercaseSeen = 0;
    //int titleSeen = 0;
    int uppercaseSeen = 0;


    WordSource source = WordSource::NONE;



    //not used... yet
    double uppercaseRatio() const
    {
        int total = uppercaseSeen + lowercaseSeen;

        if (total == 0)
            return 0.0;

        return static_cast<double>(uppercaseSeen) / total;
    }


};





class VocabularyManager
{

private:
    std::unordered_map<std::string, WordStats> vocab;



public:

    const std::unordered_map<std::string, WordStats>& getAll() const;
    std::unordered_map<std::string, WordStats>& getAll();


    void addInputWord(const std::string& word);
    void addInputWord(const WordData& word);

    void addGeneratedWord(const std::string& word);


    bool exists(const std::string& word) const;
    const WordStats& getWordStats(const std::string& word) const;

    void setWord(const std::string& word, const WordStats& stats);


    void displayVocabulary() const;

    size_t size() const;

    void clear();

    bool isEmpty() const;




    VocabularyManager& operator+= (const VocabularyManager& other);
};





