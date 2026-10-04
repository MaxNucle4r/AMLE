#include <optional>
#include <iostream>
#include <algorithm>
#include <cctype>



#include "Version.h"
#include "Persistence.h"
#include "FileSystem.h"

#include "Command Executor.h"
#include "CommandPersistence.h"
#include "CommandGeneration.h"
#include "CommandConfig.h"
#include "CommandDebug.h"
#include "CommandTraining.h"
#include "CommandPrint.h"
#include "CommandVersion.h"
#include "CommandClear.h"
#include "CommandHelp.h"










namespace Execute
{
	void Executor(const ParsedCommand& cmd, AIContext& ctx)
	{

		switch (cmd.command)
		{
			case Command::Help:
			{
				Execute::Help::showAllCommands();

				break;
			}

			case Command::Cls:
			{
				system("cls");

				break;
			}

			case Command::Version:
			{
				printVersions();

				break;
			}





			case Command::Model:
			{
				switch (cmd.target)
				{
					case Target::Load:
						Execute::Model::Load(ctx, cmd);
						break;


					case Target::Save:
						Execute::Model::Save(ctx, cmd);
						break;


					case Target::Files:
					{
						auto files = Persistence::Files::getFiles();
						Persistence::Files::printAllFilesName(files);
						break;
					}


					default:
						throw std::logic_error("Invalid 'model' target.");



				}


		
				break;

			}



			case Command::Generate:
			{
				switch (cmd.target)
				{
					case Target::Text:
						Execute::Generate::Text(cmd, ctx);
						break;


					case Target::Word:
						Execute::Generate::Word(cmd, ctx);
						break;


					default:
						throw std::logic_error("Invalid 'generate' target.");
				}

				break;
			}



			case Command::Print:
			{
				switch (cmd.target)
				{
					
					case Target::Transitions:
					case Target::Probs:
					{
						choosePrinter(cmd, ctx);
						break;
					}


					case Target::Memory:
					{
						Execute::Print::Memory(cmd, ctx.getMemory());
						break;
					}


					case Target::Vocab:
					{
						Execute::Print::Vocabulary(ctx.getVocabulary());
						break;
					}


					default:
						throw std::logic_error("Invalid 'print' target.");
					}

				break;
			}



			case Command::Config:
			{
				Execute::Config::Apply(ctx, cmd);

				break;


			}



			case Command::Debug:
			{
				Execute::Debugger::Apply(ctx, cmd);

				break;
			}



			case Command::Train:
			{
				switch (cmd.target)
				{
					case Target::Input:
						Execute::Training::initiateTraining(ctx, cmd);
						break;


					case Target::Self:
						//self train fucntion
						// maybe not the best ideea rn
						//Train::selfTrain(ctx, amount);
						break;


				default:
					throw std::logic_error("Invalid 'train' target.");


				}

				break;

			}



			case Command::Clear:
			{
				Execute::Clear::cleaner(cmd, ctx);
				break;
			}
				


			default:
				AMLE_ASSERT(false);
				throw std::runtime_error("Invalid Command");


		}


	}


}




