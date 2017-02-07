#include "MiniMap.h"

#include "Common.h"


MiniMap::MiniMap():
	mFont(nullptr),
	mSurface(nullptr),
	mMiniMap(nullptr),
	mMap(nullptr),
	mDragging(false),
	mLeftButtonDown(false),
	mMovingCamera(false),
	mHidden(false)
{
	init();
}


MiniMap::~MiniMap()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.mouseButtonDown().Disconnect(this, &MiniMap::onMouseDown);
	e.mouseButtonUp().Disconnect(this, &MiniMap::onMouseUp);
	e.mouseMotion().Disconnect(this, &MiniMap::onMouseMotion);
}


void MiniMap::init()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.mouseButtonDown().Connect(this, &MiniMap::onMouseDown);
	e.mouseButtonUp().Connect(this, &MiniMap::onMouseUp);
	e.mouseMotion().Connect(this, &MiniMap::onMouseMotion);

	Renderer& r = Utility<Renderer>::get();
	mRect.x(2);
	mRect.y(34);
}


void MiniMap::hidden(bool _b)
{
	mHidden = _b;
}


void MiniMap::adjustCamera(int x, int y)
{
	Renderer& r = Utility<Renderer>::get();
	mMap->setCamera((mMap->tileset().width() * (x - mRect.x() + 4)) - (r.width() / 2), (mMap->tileset().height() * (y - mRect.y() - 21)) - (r.height() / 2));
}


void MiniMap::onMouseDown(MouseButton b, int x, int y)
{
	if (b != BUTTON_LEFT || hidden())
		return;

	if (isPointInRect(x, y, rect().x(), rect().y(), rect().w(), 17))
		mDragging = true;

	if (isPointInRect(x, y, mRect.x() + 4, mRect.y() + 21, mMiniMap->width(), mMiniMap->height()))
	{
		mMovingCamera = true;
		adjustCamera(x, y);
	}


	mLeftButtonDown = true;
}


void MiniMap::onMouseUp(MouseButton b, int x, int y)
{
	if (b != BUTTON_LEFT || hidden())
		return;

	mDragging = false;
	mLeftButtonDown = false;
	mMovingCamera = false;
}


void MiniMap::onMouseMotion(int x, int y, int relX, int relY)
{
	if (mDragging)
	{
		mRect(mRect.x() + relX, mRect.y() + relY, mRect.w(), mRect.h());
		return;
	}

	if(mMovingCamera)
	{
		adjustCamera(x, y);
		return;
	}
}


void MiniMap::update()
{
	if (hidden())
		return;

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 180, 180, 180);
	r.drawBoxFilled(rect().x(), rect().y(), rect().w(), 16, 75, 95, 130);
	r.drawBox(rect(), 0, 0, 0);

	if (mFont)
	{
		r.drawText(*mFont, "MiniMap", mRect.x() + (mRect.w() / 2) - (mFont->width("MiniMap") / 2) - 1, rect().y() + 4, 255, 255, 255);
		r.drawText(*mFont, "MiniMap", mRect.x() + (mRect.w() / 2) - (mFont->width("MiniMap") / 2), rect().y() + 4, 255, 255, 255);
	}

	Point_2d pt(0, 0);

	Point_2d upperLeft = mMap->getGridCoords(pt);
	pt.x() += r.width();
	pt.y() += r.height();
	Point_2d lowerRight = mMap->getGridCoords(pt);

	r.drawBoxFilled(mRect.x() + 4, mRect.y() + 21, mMiniMap->width(), mMiniMap->height(), 255, 0, 255);
	r.drawImage(*mMiniMap, mRect.x() + 4, mRect.y() + 21);

	Rectangle_2d rect(mRect.x() + 4 + upperLeft.x(), mRect.y() + 21 + upperLeft.y(), lowerRight.x() - upperLeft.x(), lowerRight.y() - upperLeft.y());
	r.drawBox(rect, 255, 255, 255);
}


void MiniMap::map(Map* _m)
{
	mMap = _m;
	createMiniMap();

	mRect(mRect.x(), mRect.y(), mMap->width() + 8, mMap->height() + 25);
}

void MiniMap::update_minimap()
{
	createMiniMap();
}


void MiniMap::createMiniMap()
{
	Uint32 rmask, gmask, bmask, amask;

	// Set up channel masks.
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { rmask = 0xff000000; gmask = 0x00ff0000;	bmask = 0x0000ff00;	amask = 0x000000ff; }
	else { rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000; }

	mSurface = SDL_CreateRGBSurface(0, mMap->width(), mMap->height(), 32, rmask, gmask, bmask, amask);
	if (!mSurface)
		return;

	Color_4ub _c;
	for (int y = 0; y < mMap->height(); y++)
	{
		for (int x = 0; x < mMap->width(); x++)
		{
			Cell& cell = mMap->field().cell(x, y);

			_c = mMap->tileset().averageColor(cell.index(Cell::LAYER_BASE));
			DrawPixel(mSurface, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());

			if (cell.index(Cell::LAYER_BASE_DETAIL) != -1)
			{
				_c = mMap->tileset().averageColor(cell.index(Cell::LAYER_BASE_DETAIL));
				DrawPixel(mSurface, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}

			if (cell.index(Cell::LAYER_DETAIL) != -1)
			{
				_c = mMap->tileset().averageColor(cell.index(Cell::LAYER_DETAIL));
				DrawPixel(mSurface, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}

			if (cell.index(Cell::LAYER_FOREGROUND) != -1)
			{
				_c = mMap->tileset().averageColor(cell.index(Cell::LAYER_FOREGROUND));
				DrawPixel(mSurface, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}
		}
	}


	if (mMiniMap)
		delete mMiniMap;


	mMiniMap = new Image(mSurface->pixels, mSurface->format->BytesPerPixel, mSurface->w, mSurface->h);

	SDL_FreeSurface(mSurface);
	mSurface = nullptr;
}
