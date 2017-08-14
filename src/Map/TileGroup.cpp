#include "TileGroup.h"

#include "../Common.h"


/**
 * C'tor
 */
TileGroup::TileGroup(int width, int height, TileSetManager* tsetmanager):	mWidth(width),
																			mHeight(height),
																			mTileSetManager(tsetmanager)
{
	mTileData.resize(width * height);
}


/**
 * D'tor
 */
TileGroup::~TileGroup()
{}


/**
 * Width, in tiles, of the TileGroup.
 */
int TileGroup::width() const
{
	return mWidth;
}


/**
 * Height, in tiles, of the TileGroup.
 */
int TileGroup::height() const
{
	return mHeight;
}


/**
 * Gets a TileSet mapping index.
 */
int TileGroup::index(int x, int y) const
{
	if ((x < 0) || (x >= mWidth) || (y < 0) || (y >= mHeight))
	{
		throw std::runtime_error("TileGroup::mappingIndex(): Invalid coordinates.");
	}

	// Get the tile mapping index
	return mTileData[y * mWidth + x];
}


/**
 * Sets a TileSet mapping index.
 */
void TileGroup::index(int x, int y, int index)
{
	if ((x < 0) || (x >= mWidth) || (y < 0) || (y >= mHeight))
	{
		throw std::runtime_error("TileGroup::mappingIndex(): Invalid coordinates.");
	}

	// Set the tile mapping index
	mTileData[(y * mWidth) + x] = index;
}


/**
 * Draws the TileGroup at coordinates { x, y }
 */
void TileGroup::draw(int x, int y)
{
	// Draw all tiles in visible range
	for (int row = 0; row < mHeight; ++row)
	{
		for (int col = 0; col < mWidth; ++col)
		{
			//int tileIndex = mTileData[row * mWidth + col];
			TileSetManager::TileSetTileMapping* tileMap = &mTileSetManager->mMapping[index(col, row)];
			TileSet* tileSet = mTileSetManager->mTileSetInfo[tileMap->tileSetIndex].tileSet;
			
			if (tileSet)
			{
				tileSet->draw(tileMap->tileIndex, x + col * TILE_SIZE, y + row * TILE_SIZE);
			}
		}
	}
}
