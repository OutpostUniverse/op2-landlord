#include "TileGroup.h"


TileGroup::TileGroup(int width, int height, TileSetManager* tilesetmanager):	mTileWidth(width),
																				mTileHeight(height),
																				mTileData(new int[width * height]),
																				mTileSetManager(tilesetmanager)
{}


TileGroup::~TileGroup()
{
	if (mTileData != NULL)
	{
		delete[] mTileData;
	}
}



int TileGroup::mappingIndex(int x, int y)
{
	if ((x < 0) || (x >= mTileWidth) || (y < 0) || (y >= mTileHeight))
	{
		throw std::runtime_error("TileGroup::mappingIndex(): Invalid coordinates.");
	}

	// Get the tile mapping index
	return mTileData[y * mTileWidth + y];
}


void TileGroup::mappingIndex(int x, int y, int index)
{
	if ((x < 0) || (x >= mTileWidth) || (y < 0) || (y >= mTileHeight))
	{
		throw std::runtime_error("TileGroup::mappingIndex(): Invalid coordinates.");
	}

	// Set the tile mapping index
	mTileData[(y * mTileWidth) + x] = index;
}


void TileGroup::draw(int x, int y)
{

}
