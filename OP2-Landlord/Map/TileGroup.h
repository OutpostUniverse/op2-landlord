#pragma once

#include "TileSet.h"
#include "TileSetManager.h"

class TileGroup
{
public:
	TileGroup(int width, int height, TileSetManager* tilesetmanager);
	~TileGroup();

	int index(int x, int y) const;
	void index(int x, int y, int index);

	int width() const;
	int height() const;

	void draw(int x, int y);

private:
	typedef std::vector<int> TileIndexArray;

private:
	TileGroup(const TileGroup&) = delete;				/**< Not allowed. */
	TileGroup& operator=(const TileGroup&) = delete;	/**< Not allowed. */

private:
	int				mWidth = 0;					/**< Width of this tile group */
	int				mHeight = 0;				/**< Height of this tile group */

	TileSetManager*	mTileSetManager = nullptr;	/**<  */

	TileIndexArray	mTileData;					/**<  */
};
