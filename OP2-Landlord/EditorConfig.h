#pragma once

#include <NAS2D/Dictionary.h>
#include <string>


struct EditorConfig
{
	EditorConfig() = default;
	EditorConfig(const std::string& savePath);

	~EditorConfig();

	void saveConfig();

	char Op2FilePath[1000]{ '\0' };
	std::string UserSavePath;

	NAS2D::Dictionary Options;
};
