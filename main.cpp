#include <iostream>
#include <fstream>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// MIT License

// Copyright (c) 2024 Tomas Mark (tomas@digitalspace.name)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// masscode2md
// This project is for an export the massCode snippets to the MarkDown files
// each exported MarkDown file is located within the same folder as in MassCode defined
// Dependencies
// C++ 17
// Configuration
// Set the path to the MassCode JSON file
// std::string jsonFilePath = "/home/tomas/projects/cpp/masscode2md/db.json";

// Set Output directory
// std::string outputDir = "/home/tomas/joplin-output/";

// Why?
// I like Joplin 😉
// That's all!

int main()
{
    printf("Hello, from masscode2md!\n");

    // Set the Path to the MassCode JSON file
    std::string jsonFilePath = "/home/tomas/projects/cpp/masscode2md/db.json";
    
    // Set Output directory
    std::string outputDir = "/home/tomas/joplin-output/";

    // Remove the output directory if it exists
    if (std::filesystem::exists(outputDir))
    {
        printf("Removing output directory: %s\n", outputDir.c_str());
        std::filesystem::remove_all(outputDir);
    }

    // Load the JSON file
    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile.is_open())
    {
        printf("Failed to open JSON file: %s\n", jsonFilePath.c_str());
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
        printf("Failed to parse JSON data: %s\n", e.what());
        return 1;
    }

    // Check if output folder exists and is empty
    if (std::filesystem::exists(outputDir))
    {
        if (!std::filesystem::is_empty(outputDir))
        {
            printf("Output folder is not empty. Abort\n");
            return 1;
        }
    }
    else
    {
        printf("Output folder does not exist. Creating %s\n", outputDir.c_str());
        // Create the output folder
        if (!std::filesystem::create_directory(outputDir))
        {
            printf("Failed to create output folder. Abort\n");
            return 1;
        }
    }

    // Process the JSON data
    // ...

    // Create Folders
    std::vector<std::pair<std::string, std::string>> folderData;
    for (const auto &folders : jsonData["folders"])
    {
        std::string folderName = folders["name"];
        std::string folderId = folders["id"];
        folderData.push_back(std::make_pair(folderName, folderId));

        // Create the folder using the folderName
        // ...
        printf("Folder: %s, ID: %s\n", folderName.c_str(), folderId.c_str());

        // Replace invalid characters in folderName with a valid character
        std::replace_if(folderName.begin(), folderName.end(), [](char c)
                        { return !std::isalnum(c) && c != '_' && c != '-' && c != '+'; }, '_');

        std::string folderPath = outputDir + "/" + folderName;
        if (!std::filesystem::create_directory(folderPath))
        {
            printf("Failed to create folder: %s\n", folderPath.c_str());
            return 1;
        }

        // Update the folderName in the folderData vector
        folderData.back().first = folderName;
    }

    // Process the Snippets
    for (const auto &snippet : jsonData["snippets"])
    {
        bool isDeleted = snippet["isDeleted"];
        std::string folderId = snippet["folderId"];
        std::string name = snippet["name"];

        // Print the snippet data
        printf("Snippet: %s, Folder ID: %s\n", name.c_str(), folderId.c_str());

        if (isDeleted)
        {
            continue;
        }

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
            printf("Failed to find folder for snippet: %s\n", snippet["label"].get<std::string>().c_str());
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
                printf("Invalid content in snippet: %s\n", snippet["name"].get<std::string>().c_str());
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

            // Write the value to the file
            std::ofstream outFile(filePath);
            if (!outFile)
            {
                printf("Failed to open file: %s\n", filePath.c_str());
                continue;
            }

            if (language.empty())
            {
                language = "plaintext";
            }
            else if (language == "c_cpp")
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
}
