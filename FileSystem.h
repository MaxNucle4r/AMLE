#pragma once
#include <string>
#include <vector>
#include <filesystem>



#pragma warning(push, 0)
#include <nlohmann/json.hpp>
#pragma warning(pop)
using json = nlohmann::ordered_json;





namespace fs = std::filesystem;

namespace Persistence
{
	namespace Files
	{
		bool ensureModelsDirectory();

		std::vector<std::string> getFiles();

		void printAllFilesName(const std::vector<std::string>& files);

		fs::path getSafeModelPath(const std::string& userFile);

		std::string getDefaultName();



		namespace JSON
		{
			json readFile(const fs::path& filename);

			void writeFile(const json& j, const fs::path& filename);
		}

	}

}

