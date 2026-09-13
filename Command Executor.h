#pragma once
#include "Commands.h"
#include "AIContext.h"
#include "Debug.h"
#include "Types.h"





namespace Execute
{
	void Executor(const ParsedCommand& cmd, AIContext& ctx);
	

	namespace Training
	{
		void initiateTraining(const ParsedCommand& cmd, AIContext& ctx);
	
	}


	namespace Generate
	{
		void Text(const ParsedCommand& cmd, AIContext& ctx);

		void Word(const ParsedCommand& cmd, AIContext& ctx);
	}



	namespace Print
	{
		void TextTransitions(const AIContext& ctx);
		void TextProbabilities(const AIContext& ctx);

		void NGramTextTransitions(const AIContext& ctx, const size_t size);
		void NGramTextProbabilities(const AIContext& ctx, const size_t size);



		void CharTransitions(const AIContext& ctx);
		void CharProbabilities(const AIContext& ctx);


		void NGramCharTransitions(const AIContext& ctx, const size_t size);
		void NGramCharProbabilities(const AIContext& ctx, const size_t size);



		void Memory(const ParsedCommand& cmd,  const AIContext& ctx);

		
		void Vocabulary(const AIContext& ctx);

		//void ModelFiles();

	}



	namespace Model
	{
		void Save(const AIContext& ctx, const ParsedCommand& cmd);

		void Load(AIContext& ctx, const ParsedCommand& cmd);
	}



	namespace Config
	{
		void Apply(AIContext& ctx, const ParsedCommand& cmd);
	}



	namespace Debug
	{
		void Apply(AIContext& ctx, const ParsedCommand& cmd);
	}
	

}




