#pragma once

#include <map>
#include <string>

struct EditorConfig
{
	EditorConfig() = default;
	EditorConfig(const std::string& savePath);

	~EditorConfig();

	void saveConfig();

	std::string& operator[](const std::string& key);

	bool contains(const std::string& key) const;

private:
	std::map<std::string, std::string> Options;
};
