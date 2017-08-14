#include "MiniMap.h"

#include "Common.h"

using namespace NAS2D;

MiniMap::MiniMap()
{
	_init();
}


MiniMap::~MiniMap()
{}


void MiniMap::_init()
{
	position(2, 34);
	size(70, 70);
	text("Mini Map");
}


void MiniMap::adjustCamera(int x, int y)
{
	Renderer& r = Utility<Renderer>::get();
	mMap->setCamera((TILE_SIZE * (x - (int)rect().x() + 4)) - ((int)r.width() / 2),(TILE_SIZE * (y - (int)rect().y() - 21)) - ((int)r.height() / 2));
}


void MiniMap::mouseDown(EventHandler::MouseButton b, int x, int y)
{
	if (b != EventHandler::BUTTON_LEFT) { return; }
	
	mLeftButtonDown = true;

	if (!mMiniMap) { return; }

	if (isPointInRect(x, y, rect().x() + 4, rect().y() + 21, mMiniMap->width(), mMiniMap->height()))
	{
		mMovingCamera = true;
		adjustCamera(x, y);
	}

}


void MiniMap::mouseUp(EventHandler::MouseButton b, int x, int y)
{
	if (b != EventHandler::BUTTON_LEFT) { return; }

	mLeftButtonDown = false;
	mMovingCamera = false;
}


void MiniMap::mouseMotion(int x, int y, int relX, int relY)
{
	if (dragging()) { return; }

	if(mMovingCamera)
	{
		adjustCamera(x, y);
		return;
	}
}


void MiniMap::draw()
{
	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 180, 180, 180);
	r.drawBoxFilled((float)rect().x(), (float)rect().y(), (float)rect().width(), 16, 75, 95, 130);
	r.drawBox(rect(), 0, 0, 0);


	r.drawBoxFilled(rect().x() + 5, rect().y() + 21, (float)mMiniMap->width(), (float)mMiniMap->height(), 255, 0, 255);
	r.drawImage(*mMiniMap, rect().x() + 5, rect().y() + 21);

	mViewRect(rect().x() + 5 + (mMap->cameraPosition().x() / TILE_SIZE), rect().y() + 21 + (mMap->cameraPosition().y() / TILE_SIZE), static_cast<int>(r.width() / TILE_SIZE), static_cast<int>(r.height() / TILE_SIZE));
	r.drawBox(mViewRect, 255, 255, 255);
}


void MiniMap::update_minimap()
{
	createMiniMap();
	size(mMiniMap->width() + 10, mMiniMap->height() + 26);
}


void MiniMap::createMiniMap()
{
	if (!mMap) { throw std::runtime_error("MiniMap::createMiniMap(): mMap not set!"); }

	Uint32 rmask, gmask, bmask, amask;

	// Set up channel masks.
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { rmask = 0xff000000; gmask = 0x00ff0000;	bmask = 0x0000ff00;	amask = 0x000000ff; }
	else { rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000; }

	mSurface = SDL_CreateRGBSurface(0, mMap->width(), mMap->height(), 32, rmask, gmask, bmask, amask);
	if (!mSurface) { return; }

	Color_4ub _c;
	for (int y = 0; y < mMap->height(); y++)
	{
		for (int x = 0; x < mMap->width(); x++)
		{
			_c = mMap->tile_color(x, y);

			DrawPixel(mSurface, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
		}
	}

	if (mMiniMap) { delete mMiniMap; }

	mMiniMap = new Image(mSurface->pixels, mSurface->format->BytesPerPixel, mSurface->w, mSurface->h);

	SDL_FreeSurface(mSurface);
	mSurface = nullptr;
}
