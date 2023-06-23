#pragma once


#include <string>


struct EditorConfig
{
	EditorConfig() = default;
	EditorConfig(const std::string& savePath);

	~EditorConfig();

	void saveConfig();

	char Op2FilePath[1000]{ '\0' };
	std::string UserSavePath;
};
