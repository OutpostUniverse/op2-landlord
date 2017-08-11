#pragma once

#include <NAS2D/NAS2D.h>

#include "color_table.h"

class TileSet
{
public:
	const std::string& name() { return mTileSetName; }

public:
	TileSet(const std::string& filename);
	~TileSet();

	int tileCount() const;
	const NAS2D::Color_4ub& color(int index) const;

	void draw(int index, int x, int y);

private:
	TileSet() = delete;								// Not allowed
	TileSet(const TileSet&) = delete;				// Not allowed
	TileSet& operator=(const TileSet&) = delete;	// Not allowed

private:
	std::string			mTileSetName;

	NAS2D::Image		mTileSetImage;

	int					mTileCount = 0;

	const ColorTable&	mColorTable;
};
