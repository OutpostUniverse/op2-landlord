#pragma once

#include "TileSet.h"
#include "TileSetManager.h"

class TileGroup
{
public:
	TileGroup(int tileWidth, int tileHeight, TileSetManager* tilesetmanager);
	~TileGroup();

	int mappingIndex(int x, int y);
	void mappingIndex(int tileX, int tileY, int tileMappingIndex);

	void draw(int x, int y);

private:
	int				mTileWidth = 0;				/**< Width of this tile group */
	int				mTileHeight = 0;			/**< Height of this tile group */

	int*			mTileData = nullptr;		/**< Array of mapping indexes */

	TileSetManager*	mTileSetManager = nullptr;
};
