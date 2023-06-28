#pragma once

#include <string>


struct SDL_Window;
struct SDL_Renderer;


class Gui
{
public:
	Gui() = delete;
	Gui(const std::string& settingsPath, SDL_Window* window, SDL_Renderer* renderer);

	~Gui();

	void newFrame();
	void endFrame();

private:
	const std::string mIniSavePath;
};
