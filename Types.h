#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>





namespace Hashes 
{
    struct WordPairHash
    {
        size_t operator()(const std::pair<std::string, std::string>& p) const
        {
            return std::hash<std::string>{}(p.first) ^
                (std::hash<std::string>{}(p.second) << 1);
        }

    };



    struct CharPairHash
    {
        size_t operator()(const std::pair<char, char>& p) const
        {
            return std::hash<char>{}(p.first) ^
                (std::hash<char>{}(p.second) << 1);
        }

    };



    // 0x9e3779b9 in HEX ----- 2654435769 in decimals
    // which is 2^32 / phi
    // (phi = golden ration)
    //used for hasing for being "the most irational" irational number... interesitng...
    // any irational number is ok
    //but goldoen ratio has the worst rational aproximation
    // phi ~ (1+ sqrt(5) )/2
    // pi ~ 22/7
    //less clustering when hashing, less lining up with repeating patters
    struct VectorHashString
    {
        size_t operator()(const std::vector<std::string>& v) const
        {
            size_t seed = 0;

            for (const auto& word : v)
            {
                seed ^= std::hash<std::string>{}(word)
                    +0x9e3779b9
                    + (seed << 6)
                    + (seed >> 2);
            }

            return seed;
        }
    };



    struct VectorHashChar
    {
        size_t operator()(const std::vector<char>& v) const
        {
            size_t seed = 0;

            for (const auto& c: v)
            {
                seed ^= std::hash<char>{}(c)
                    +0x9e3779b9
                    + (seed << 6)
                    + (seed >> 2);
            }

            return seed;
        }
    };

}





namespace TextTypes
{
    
    using NextWordMap = std::unordered_map<std::string, int>;


    // LEGACY

    using WordPair = std::pair<std::string, std::string>; 


    using TransitionMap =
        std::unordered_map<WordPair, NextWordMap, Hashes::WordPairHash>;

    using
        ProbabilityMap =
        std::unordered_map<
        WordPair,
        std::unordered_map<std::string, double>,
        Hashes::WordPairHash>;



    // 4GRAM - NGRAM

    using Ngram = std::vector<std::string>;


    using NgramTransitionMap =
        std::unordered_map<Ngram, NextWordMap, Hashes::VectorHashString>;


    using NgramProbabilityMap =
        std::unordered_map <
        Ngram,
        std::unordered_map<std::string, double>,
        Hashes::VectorHashString>;

}
 


namespace CharTypes
{
    using NextCharMap = std::unordered_map<char, int>;


    // LEGACY
    using CharPair = std::pair<char, char>;


    using TransitionMap =
        std::unordered_map<CharPair, NextCharMap, Hashes::CharPairHash>;


    using ProbabilityMap =
        std::unordered_map<
        CharPair,
        std::unordered_map<char, double>,
        Hashes::CharPairHash>;



    // 4GRAM - NGRAM

    using Ngram = std::vector<char>;


    using NgramTransitionMap =
        std::unordered_map<Ngram, NextCharMap, Hashes::VectorHashChar>;


    using NgramProbabilityMap =
        std::unordered_map <
        Ngram,
        std::unordered_map<char, double>,
        Hashes::VectorHashChar>;





}





