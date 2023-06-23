#include "EditorConfig.h"

#include <NAS2D/Dictionary.h>


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

	void loadConfig(const std::string filePath)
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

}


EditorConfig::EditorConfig(const std::string& savePath) : 
	UserSavePath{ savePath }
{
	loadConfig(UserSavePath + "editor.json");
}


EditorConfig::~EditorConfig()
{
	saveConfig();
}


void EditorConfig::saveConfig()
{
}
