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

	std::map<std::string, std::string> loadConfigFromFile(const std::string filePath)
	{
		if (!std::filesystem::exists(filePath))
		{
			std::ofstream file{ filePath, std::ios::out | std::ios::binary };
			if (!file)
			{
				throw std::runtime_error("Error opening file for writing: " + filePath + " : " + errorDescription());
			}

			return {};
		}

		try
		{
			std::ifstream file{ filePath };
			json optionsJson = json::parse(file);
			std::map<std::string, std::string> options;

			for (auto& [key, value] : optionsJson.items())
			{
				options.emplace(key, value);
			}

			return options;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		return {};
	}


	void saveConfigToFile(const std::string& filePath, const std::map<std::string, std::string>& options)
	{
		std::ofstream file{ filePath, std::ios::out | std::ios::binary };

		if (!file)
		{
			throw std::runtime_error("Error opening file for writing: " + filePath + " : " + errorDescription());
		}

		json jsonMap(options);

		file << jsonMap;
		file.close();
	}

}


EditorConfig::EditorConfig(const std::string& savePath)
{
	Options = loadConfigFromFile(savePath + "editor.json");
	Options.emplace("UserSavePath", savePath);
}


EditorConfig::~EditorConfig()
{
	saveConfig();
}


void EditorConfig::saveConfig()
{
	saveConfigToFile(Options.at("UserSavePath") + "editor.json", Options);
}


std::string& EditorConfig::operator[](const std::string& key)
{
	return Options[key];
}


bool EditorConfig::contains(const std::string& key) const
{
	return Options.contains(key);
}
