#include <iostream>
#include <fstream>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// deleted snippets are not converted despite being in the JSON file

// if you experiment with the code, you may want to change this to false and back to true to avoid deleting the output folder
#define ERASE_OUTPUT_DIR_AT_STARTUP true

int main(int argc, char *argv[], char *env[])
{
    std::cout << "masscode2md" << std::endl;

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <input_json_path> <output_directory>\n";
        return 1;
    }

    // Set the path to the JSON file
    std::string jsonFilePath = argv[1];

    // Set the output directory
    std::string outputDir = argv[2];

    if (ERASE_OUTPUT_DIR_AT_STARTUP)
    {
        if (std::filesystem::exists(outputDir))
        {
            std::cout << "ERASE_OUTPUT_DIR_AT_STARTUP is true\n";
            std::cout << "Output directory " << outputDir << "exists. Removing it." << std::endl;
            std::filesystem::remove_all(outputDir);
        }
    }

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile.is_open())
    {
        std::cout << "Failed to open JSON file: " << jsonFilePath << std::endl;
        return 1;
    }

    // Parse the JSON data
    json jsonData;
    try
    {
        jsonFile >> jsonData;
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to parse JSON data: " << e.what() << std::endl;
        return 1;
    }

    // Check if output folder exists and is empty
    if (std::filesystem::exists(outputDir))
    {
        if (!std::filesystem::is_empty(outputDir))
        {
            std::cout << "Output folder is not empty. Abort" << std::endl;
            return 1;
        }
    }
    else
    {
        printf("Output folder does not exist. Creating %s\n", outputDir.c_str());
        // Create the output folder
        if (!std::filesystem::create_directory(outputDir))
        {
            std::cout << "Failed to create output folder. Abort" << std::endl;
            return 1;
        }
    }

    // Create Folders
    std::vector<std::pair<std::string, std::string>> folderData;
    for (const auto &folders : jsonData["folders"])
    {
        std::string folderName = folders["name"];
        std::string folderId = folders["id"];
        folderData.push_back(std::make_pair(folderName, folderId));

        // Create the folder using the folderName
        std::cout << "Folder Name: " << folderName << ", Folder ID: " << folderId << std::endl;

        // Replace invalid characters in label with a valid character
        std::replace_if(folderName.begin(), folderName.end(), [](char c)
                        { return !std::isalnum(c) && c != '_' && c != '-' && c != '+'; }, '_');

        std::string folderPath = outputDir + "/" + folderName;
        if (!std::filesystem::create_directory(folderPath))
        {
            std::cout << "Failed to create folder: " << folderPath << std::endl;
            return 1;
        }

        // Update the folderName in the folderData vector
        folderData.back().first = folderName;
    }

    // Process the Snippets
    for (const auto &snippet : jsonData["snippets"])
    {

        // skip deleted snippets
        if (snippet["isDeleted"])
        {
            continue;
        }

        std::string folderId = snippet["folderId"];
        std::string name = snippet["name"];

        // Print the snippet data
        std::cout << "Snippet: " << name << ", Folder ID: " << folderId << std::endl;

        // Find the folderName for this folderId
        std::string folderName;
        for (const auto &folder : folderData)
        {
            if (folder.second == folderId)
            {
                folderName = folder.first;
                break;
            }
        }

        if (folderName.empty())
        {
            std::cout << "Failed to find folder for snippet: " << snippet["label"].get<std::string>() << std::endl;
            continue;
        }

        // Process each content in the snippet and add id of the content to the end of the file name
        int id = 0;
        for (const auto &content : snippet["content"])
        {

            std::string label = content["label"];
            std::string value = content["value"];
            std::string language = content["language"];

            if (label.empty() || value.empty())
            {
                std::cout << "Invalid content in snippet: " << snippet["name"].get<std::string>() << std::endl;
                continue;
            }

            // Replace invalid characters in label with a valid character
            std::replace_if(name.begin(), name.end(), [](char c)
                            { return !std::isalnum(c) && c != '_' && c != '-' && c != '+'; }, '_');

            // Create the file name depending on the id
            if (id > 0)
            {
                name = name + "_" + std::to_string(id);
            }

            std::string filePath = outputDir + "/" + folderName + "/" + name + ".md";

            // Write the content to the file
            std::ofstream outFile(filePath);
            if (!outFile)
            {
                std::cout << "Failed to open file: " << filePath << std::endl;
                continue;
            }

            // Change the language to cpp if it is c_cpp to match the markdown language
            if (language == "c_cpp")
            {
                language = "cpp";
            }

            outFile << "```" << language << '\n';
            outFile << value;
            outFile << '\n'
                    << "```" << '\n';

            outFile.close();
        }
    }
    std::cout << "Done" << std::endl;
}
