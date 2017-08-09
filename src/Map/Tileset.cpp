#include "Tileset.h"

#include <iostream>

#include "NAS2D/NAS2D.h"

using namespace std;

const Color_4ub COLOR_CLEAR(0, 0, 0, 0);

/**
 * C'tor
 */
Tileset::Tileset(const string& path, int tileWidth, int tileHeight):	mTileset(path),
																		mTsetPath(path),
																		mTileDimensions(tileWidth, tileHeight),
																		mTileHalfDimensions(tileWidth / 2, tileHeight / 2),
																		mTilesetDimensions(0, 0)
{
	init();
}


/**
 * Convenience function that gets an origin point for a tile in
 * a tileset given a tile index.
 * 
 * \note	Does not check to see if the index is outside the bounds
 *			of a tileset image.
 */
const Rectangle_2d Tileset::getTsetCoordsFromIndex(int index) const
{
	int x = (index % mTilesetDimensions.x()) * mTileDimensions.x();
	int y = (index / mTilesetDimensions.x()) * mTileDimensions.y();

	return Rectangle_2d(x, y, mTileDimensions.x(), mTileDimensions.y());
}


/**
 * Draws an indexed tile at (X, Y) on the screen.
 * 
 * \note	Does not check to see if the index is outside the bounds
 *			of a tileset image.
 */
void Tileset::drawTile(int index, int x, int y)
{
	const Rectangle_2d rect = getTsetCoordsFromIndex(index);
	Utility<Renderer>::get().drawSubImage(mTileset, x, y, rect.x(), rect.y(), rect.width(), rect.height());
}


/**
 * Draws tiles to an Image instead of the screen.
 */
void Tileset::drawTileToImage(Image& img, int index, int x, int y)
{
//	const Rectangle_2d rect = getTsetCoordsFromIndex(index);
//	Utility<Renderer>::get().drawImageToImage(mTileset, rect, img, Point_2d(x, y));
}


void Tileset::drawTileColorPalette(int x, int y, int cell_size, int columns)
{

	Rectangle_2df rect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(cell_size), static_cast<float>(cell_size));

	int col = 0;
	for(int i = 0; i < numTiles(); i++)
	{
		Utility<Renderer>::get().drawBoxFilled(rect, mAverageTileColorsList[i].red(), mAverageTileColorsList[i].green(), mAverageTileColorsList[i].blue(), mAverageTileColorsList[i].alpha());

		col++;
		
		if(col >= columns)
		{
			col = 0;
			rect.y(rect.y() + cell_size);
		}

		rect.x(x + (col * cell_size));
	}
}


/**
 * Performs basic processing on the base tileset image and gets
 * several internal variables ready for use.
 */
void Tileset::init()
{
	if(!mTileset.loaded())
	{
		std::cout << "Tileset image did not loaded properly." << endl;
		return;
	}

	mTilesetDimensions.x(mTileset.width() / mTileDimensions.x());
	mTilesetDimensions.y(mTileset.height() / mTileDimensions.y());

	fillTileColorList();
}


const Color_4ub& Tileset::averageColor(int index)
{
	if(index < mAverageTileColorsList.size())
		return mAverageTileColorsList[index];

	return COLOR_CLEAR;
}


void Tileset::fillTileColorList()
{
	mAverageTileColorsList.resize(numTiles());

	for(int i = 0; i < numTiles(); i++)
	{
		const Rectangle_2d& rect = getTsetCoordsFromIndex(i);
		int r = 0, g = 0, b = 0, a = 0;
		int pixel_count = 0;

		for(int y = 0; y < rect.height(); y++)
		{
			for(int x = 0; x < rect.width(); x++)
			{
				Color_4ub& c = mTileset.pixelColor(rect.x() + x, rect.y() + y);

				if(c.alpha() > 235)
				{
					r += c.red();
					g += c.green();
					b += c.blue();
					a += c.alpha();
					pixel_count++;
				}
			}
		}

		if(pixel_count > 0)
		{
			r /= pixel_count;
			g /= pixel_count;
			b /= pixel_count;
			a /= pixel_count;

			mAverageTileColorsList[i](r, g, b, 255);
		}
		else
			mAverageTileColorsList[i] = COLOR_CLEAR;

	}
}