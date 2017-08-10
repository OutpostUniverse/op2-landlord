#include "TileSet.h"
//#include "GlobalFunctions.h"

using namespace NAS2D;


TileSet::TileSet(const std::string& filename): mTileSetImage(filename + ".png"), mTileCount(mTileSetImage.height() / 32)
{
	if (mTileCount == 0)
	{
		throw std::runtime_error("TileSet: Unable to load '" + filename + "'");
	}
	
	buildTileColorTable();
}


TileSet::~TileSet()
{}


int TileSet::tileCount()
{
	return mTileCount;
}


void TileSet::draw(int index, int x, int y)
{
	if (index * 32 > mTileCount)
	{
		// Should probably throw an exception here instead of silently failing as this would basically be an error.
		return;
	}

	Utility<Renderer>::get().drawSubImage(mTileSetImage, static_cast<float>(x), static_cast<float>(y), 0, static_cast<float>(index * 32), 32, 32);
}


const TileSet::Color& TileSet::color(int index)
{
	if (index < 0 || index >= mTileCount)
	{
		throw std::runtime_error("TileSet::color(): Index out of range.");
	}

	return mColorTable[index];
}


void TileSet::buildTileColorTable()
{
	/*
	mColorTable.resize(mTileCount);

	int pixel_count = 32 * 32;

	for (int i = 0; i < mTileCount; i++)
	{
		int r = 0, g = 0, b = 0, a = 0;

		for (int y = 0; y < 32; y++)
		{
			for (int x = 0; x < 32; x++)
			{
				Color_4ub& c = mTileSetImage.pixelColor(x, (i * 32) + y);

				r += c.red();
				g += c.green();
				b += c.blue();
			}
		}

		r /= pixel_count;
		g /= pixel_count;
		b /= pixel_count;

		mColorTable[i] = Color(r, g, b);
	}
	*/
}
