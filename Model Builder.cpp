#include <iostream>
#include <sstream>


#include "Model Builder.h"
#include "Text Processing.h"





namespace Model::Text
{
	void buildTextTransitions(TextTypes::TransitionMap& transitions, 
        const std::vector<std::string>& tokens)
	{
        if (tokens.empty())
        {
            throw std::runtime_error("Emprty tokens");
        }



        std::vector<std::string> preparedTokens;

        preparedTokens.reserve(tokens.size() + 3);

        preparedTokens.insert(preparedTokens.end(), 2, "<START>");
		preparedTokens.insert(preparedTokens.end(), tokens.begin(), tokens.end()); 

        preparedTokens.push_back("<END>");



        for (size_t j = 0; j + 2 < preparedTokens.size(); j++)
        {
            transitions[{preparedTokens[j], preparedTokens[j + 1]}][preparedTokens[j + 2]]++;
        }


	}


}





namespace Model::Word
{
    void buildCharTransitions(CharTypes::TransitionMap& transitions, 
        const std::vector<std::string>& tokens)
    {
        if (tokens.empty())
        {
            throw std::runtime_error("Emprty tokens");
        }


        for (const auto& t : tokens)
        {
            std::vector<char> chars = { '$', '$' };


            for (unsigned char c : t)
            {
                if(std::isalpha(c))
                    chars.push_back(c);
            }

            chars.push_back('#');


            if (chars.size() == 3)
                continue;



            for (size_t i = 0; i + 2 < chars.size(); ++i)
            {
                transitions[{chars[i], chars[i + 1]}][chars[i + 2]]++;
            }


        }
        


    }


}




namespace Model::Text
{
    void buildTextTransitionsNGram(TextTypes::NgramTransitionMap& transitions, const size_t contextSize,
        const std::vector<std::string>& tokens
        )
    {
        if (tokens.empty())
        {
            throw std::runtime_error("Emprty tokens");
        }



		std::vector<std::string> preparedTokens;

		preparedTokens.reserve(tokens.size() + contextSize + 1);

		preparedTokens.insert(preparedTokens.end(), contextSize, "<START>");
		preparedTokens.insert(preparedTokens.end(), tokens.begin(), tokens.end());

		preparedTokens.push_back("<END>");

        

        for (size_t i = 0; i + contextSize < preparedTokens.size(); ++i)
        {
            TextTypes::Ngram context(
                preparedTokens.begin() + i,
                preparedTokens.begin() + i + contextSize
            );


            transitions[context][preparedTokens[i + contextSize]]++;


        }


    }


}





namespace Model::Word
{
    void buildCharTransitionsNGram(CharTypes::NgramTransitionMap& transitions, const size_t contextSize,
        const std::vector<std::string>& tokens)
    {
        if (tokens.empty())
        {
            throw std::runtime_error("Emprty tokens");
        }

        for (const auto& t : tokens)
        {
            std::vector<char> chars(contextSize, '$');


            for (unsigned char c : t)
            {
                if (std::isalpha(c))
                    chars.push_back(c);
            }

            chars.push_back('#');


            if (chars.size() <= contextSize)
                continue;


            for (size_t i = 0; i + contextSize < chars.size(); ++i)
            {
                CharTypes::Ngram context(
                    chars.begin() + i,
                    chars.begin() + i + contextSize
                );


                transitions[context][chars[i + contextSize]]++;


            }


        
        }
    
    
    }

}


