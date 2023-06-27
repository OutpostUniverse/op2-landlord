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
	StringTable() = default;
	
	void load(const std::string& filepath);

	const std::string& operator[](const StringName name) const;
};
