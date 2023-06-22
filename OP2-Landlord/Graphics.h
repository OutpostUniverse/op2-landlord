#pragma once


#include <SDL2/SDL.h>

#include "NAS2D/Math/Rectangle.h"
#include "NAS2D/Math/Point.h"
#include "NAS2D/Math/Vector.h"

#include "NAS2D/Renderer/Color.h"


class Graphics
{
public:

	struct Texture
	{
		SDL_Texture* texture{ nullptr };
		SDL_Rect area{};

		NAS2D::Vector<int> dimensions{};
	};

public:
	Graphics() = delete;
	Graphics(NAS2D::Vector<int> windowSize);

	void drawColor(const NAS2D::Color drawColor);

	void clear();
	void present();

	Texture loadTexture(const std::string& filename);

	SDL_Window* window() { return mWindow; }
	SDL_Renderer* renderer() { return mRenderer; }

	const NAS2D::Vector<int>& size() { return mWindowSize; }

private:
	void init();


	NAS2D::Vector<int> mWindowSize{};

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	Uint32 mWindowId = 0;
};
