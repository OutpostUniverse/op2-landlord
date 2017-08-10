#pragma once

#include <NAS2D/NAS2D.h>


class TileSet
{
public:
	struct Color
	{
		Color() {}
		Color(int r, int g, int b) : red(r), green(g), blue(b) {}

		int red = 0;
		int green = 0;
		int blue = 0;
	};

	typedef std::vector<Color>	ColorTable;

public:
	TileSet(const std::string& filename);
	~TileSet();

	int tileCount();

	void draw(int index, int x, int y);

	const Color& color(int index);

private:
	void buildTileColorTable();

private:
	TileSet() {};

private:
	NAS2D::Image	mTileSetImage;
	int				mTileCount = 0;

	ColorTable		mColorTable;
};
