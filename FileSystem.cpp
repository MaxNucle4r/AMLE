#include <iostream>
#include <fstream>



#include "Version.h"
#include "FileSystem.h"
#include "TimeStamp.h"





fs::path Persistence::Files::getSafeModelPath(const std::string& userFile)
{
    const fs::path root = 
        fs::weakly_canonical(Version::ModelsDirectory);

    fs::path full = root / userFile;
    full.replace_extension(".amle");

    // weakly canonical cleans up the path
    full = fs::weakly_canonical(full);

    
    if (full.parent_path() != root)
    {
        throw std::invalid_argument("Path escape blocked");
    }

    return full;
}



std::vector<std::string> Persistence::Files::getFiles()
{
    std::vector<std::string> files;

    //u can replace "models" with "." to look in the current project folder
    for (const auto& file : fs::directory_iterator(Version::ModelsDirectory))
    {
        if (file.path().extension() == ".amle")
        {
            files.push_back(file.path().filename().string());
        }
    }

    return files;
}



void Persistence::Files::printAllFilesName(const std::vector<std::string>& files)
{
    if (files.empty())
    {
        std::cout << "NO MODELS FOUND.\n";
        return;
    }

    std::cout << "AVAILABLE MODELS:\n";

    for (const auto& f : files)
        std::cout << "> " << f << "\n";
}



std::string Persistence::Files::getDefaultName()
{
    return Time::getTimestamp() + ".amle";
}





namespace Persistence::Files
{

    bool ensureModelsDirectory()
    {

        if (!fs::exists(Version::ModelsDirectory))
        {
            std::cout << " 'models' folder does not exist, creating one... \n";

            if (!fs::create_directory(Version::ModelsDirectory))
            {

                throw std::runtime_error("Unable to create directory\n");
            }

            return true;
        }


        if(!fs::is_directory(Version::ModelsDirectory))
        {
            throw std::runtime_error("'models' has been found as a file instead of directory. delete/rename the file\n");
        }


        return true;


    }


}





json Persistence::Files::JSON::readFile(const fs::path& filename)
{
    std::ifstream in(filename);

    if (!in)
        throw std::runtime_error("Unable to open file for reading.");

    json j;
    in >> j;

    return j;
}


void Persistence::Files::JSON::writeFile(
    const json& j,
    const fs::path& filename)
{
    std::ofstream out(filename);

    if (!out)
        throw std::runtime_error("Unable to open file for writing.");

    out << j.dump(4);
}



