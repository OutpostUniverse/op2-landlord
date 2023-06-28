#pragma once


#include <string>


class StringTable
{
public:
	enum class StringName
	{
		EmptyDir,
		EmptyDirMsg,
		MissingAssets,
		MissingAssetsMsg
	};

public:
	StringTable() = delete;
	StringTable(const std::string& filepath);

	const std::string& operator[](const StringName name) const;
};
