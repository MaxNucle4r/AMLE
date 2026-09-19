#include "CommandPersistence.h"
#include "Persistence.h"
#include "FileSystem.h" 





static std::string getFileName(const ParsedCommand& cmd)
{
	std::string filename;

	auto files = Persistence::Files::getFiles();


	auto file = cmd.options.find("filename");

	if (cmd.target == Target::Save && file == cmd.options.end())
	{
		filename = Persistence::Files::getDefaultName();

		std::cout << "No filename provided for saving\n"
			<< "Default name will be chosen instead\n"
			<< filename << "\n";
	}

	else if (cmd.target == Target::Save && file != cmd.options.end())
	{
		filename = file->second;


		if (auto it = std::find(files.begin(), files.end(), filename);
			it != files.end())
		{
			throw std::runtime_error("File with name: *" + filename + "* already exists");
		}

	}



	if (cmd.target == Target::Load && file == cmd.options.end())
	{
		//take the latest one ... hopefully xD
		//seems like... windows is not such a nice guy
		//it will get the files
		//just not in a proper order
		//fine for now...

		if (files.empty())
			throw std::runtime_error("No saved models found.");


		filename = files.back();

		//MUST change this later
		//u added j["metadata"]["lastUsed"]
		//for save format 1... good luck 
		//for save format 2 use that
		std::cout << "NO filename provided for loading\n"
			<< "Using an automatically selected model\n"
			<< filename << "\n";

	}

	else if (cmd.target == Target::Load && file != cmd.options.end())
	{
		filename = file->second;

		if (auto it = std::find(files.begin(), files.end(), filename);
			it == files.end())
		{
			throw std::runtime_error("Invalid file for loading\n");
		}

	}



	return filename;

}



namespace Execute::Model
{
	void Save(const AIContext& ctx, const ParsedCommand& cmd)
	{

		std::string filename = getFileName(cmd);

		Persistence::save(ctx, filename);


	}



	void Load(AIContext& ctx, const ParsedCommand& cmd)
	{
		std::string filename = getFileName(cmd);

		Persistence::load(ctx, filename);
	}


}