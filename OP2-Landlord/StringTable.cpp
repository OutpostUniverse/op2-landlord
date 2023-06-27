#include "StringTable.h"


#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>


#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace
{
	std::map<std::string, StringTable::StringName> StringToStringNameTable{
		{"EmptyDir", StringTable::StringName::EmptyDir },
		{"EmptyDirMsg", StringTable::StringName::EmptyDirMsg },
		{"MissingAssets", StringTable::StringName::MissingAssets },
		{"MissingAssetsMsg", StringTable::StringName::MissingAssetsMsg }
	};

	std::map<StringTable::StringName, std::string> StringHashTable;

	void loadStringTable(const std::string& filepath)
	{
		if (!std::filesystem::exists(filepath))
		{
				constexpr auto errorDesc = []() { return std::error_code{ errno, std::generic_category() }.message(); };
				throw std::runtime_error("Error opening file for reading: " + filepath + " : " + errorDesc());
		}

		std::ifstream file{filepath};
		json strings = json::parse(file);

		for (auto& [key, value] : strings.items())
		{
			StringHashTable.emplace(StringToStringNameTable.at(key), value);
		}
	}
}


void StringTable::load(const std::string& filepath)
{
	loadStringTable(filepath);
}


const std::string& StringTable::operator[](const StringName name) const
{
	return StringHashTable.at(name);
}
