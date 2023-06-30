#pragma once


#include <SDL2/SDL.h>
#include <string>

#include "imgui.h"


class Graphics
{
public:

	struct Texture
	{
		SDL_Texture* texture{ nullptr };
		SDL_Rect area{};

		ImVec2 dimensions{};
	};

public:
	Graphics() = delete;
	Graphics(ImVec2 windowSize);

	void drawColor(const ImColor& drawColor);

	void clear();
	void present();

	Texture loadTexture(const std::string& filename) const;
	Texture loadTexture(const void* buffer, const size_t bufferSize) const;

	SDL_Window* window() { return mWindow; }
	SDL_Renderer* renderer() { return mRenderer; }

	const ImVec2& size() { return mWindowSize; }

private:
	void init();


	ImVec2 mWindowSize{};

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	Uint32 mWindowId = 0;
};
