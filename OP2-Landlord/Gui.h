#pragma once

#include <string>

#include "EditorConfig.h"
#include "FileIo.h"
#include "Graphics.h"
#include "StringTable.h"


struct SDL_Window;
struct SDL_Renderer;


class Gui
{
public:
	enum class AppState
	{
		InitialSetup,
		CreateOrLoadMap
	};

public:
	Gui() = delete;
	Gui(StringTable& table, EditorConfig& config, Graphics& graphics, const std::string& settingsPath);

	~Gui();

	void newFrame();
	void endFrame();

	bool initialSetup();
	bool createOrLoadMap();

private:
	const StringTable& mStringTable;
	const std::string mIniSavePath;

	Graphics& mGraphics;
	EditorConfig& mConfig;

	FileIo mFileIo;
};
