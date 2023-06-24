#include "EditorConfig.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include <iostream>


using json = nlohmann::json;


namespace
{
	std::string errorDescription()
	{
		return std::error_code{ errno, std::generic_category() }.message();
	}

	void loadConfigFromFile(const std::string filePath)
	{
		if (!std::filesystem::exists(filePath))
		{
			std::ofstream file{ filePath, std::ios::out | std::ios::binary };
			if (!file)
			{
				throw std::runtime_error("Error opening file for writing: " + filePath + " : " + errorDescription());
			}

			return;
		}

		try
		{
			std::ifstream file(filePath);
			json data = json::parse(file);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}


	void saveConfigToFile(const std::string& filePath)
	{
		std::ofstream file;
		if (!std::filesystem::exists(filePath))
		{
			file = std::ofstream{ filePath, std::ios::out | std::ios::binary };
			if (!file)
			{
				throw std::runtime_error("Error opening file for writing: " + filePath + " : " + errorDescription());
			}
		}


	}

}


EditorConfig::EditorConfig(const std::string& savePath) : 
	UserSavePath{ savePath }
{
	loadConfigFromFile(UserSavePath + "editor.json");
}


EditorConfig::~EditorConfig()
{
	saveConfig();
}


void EditorConfig::saveConfig()
{
	saveConfigToFile(UserSavePath + "editor.json");
}
