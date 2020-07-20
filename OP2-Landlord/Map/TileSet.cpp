#include "TileSet.h"

using namespace NAS2D;

/**
 * C'Tor
 */
TileSet::TileSet(const std::string& filename):	mTileSetName(filename),
												mTileSetImage(filename + ".png"),
												mTileCount(mTileSetImage.height() / 32),
												mColorTable(selectColorTable(filename))
{
	if (mTileCount == 0)
	{
		throw std::runtime_error("TileSet: Unable to load '" + filename + "'");
	}
}


/**
 * D'Tor
 */
TileSet::~TileSet()
{}


/**
 * 
 */
int TileSet::tileCount() const
{
	return mTileCount;
}


/**
 * 
 */
const NAS2D::Color& TileSet::color(int index) const
{
	if (index < 0 || index >= mTileCount)
	{
		throw std::runtime_error("TileSet::color(): Index out of range.");
	}

	return mColorTable[index];
}


/**
 * 
 */
void TileSet::draw(int index, int x, int y)
{
	if (index > mTileCount)
	{
		// Should probably throw an exception here instead of silently failing as this would basically be an error.
		return;
	}

	Utility<Renderer>::get().drawSubImage(mTileSetImage, NAS2D::Point{x, y}, NAS2D::Rectangle{0, index * 32, 32, 32});
}



/**
 * Builds a color table of each tile's average color.
 */
 /*
void TileSet::buildTileColorTable()
{

	ofstream _file_header;
	_file_header.open("color_table.h", ios::app);
	_file_header << endl << endl << "extern ColorTable " << name() << "_color_table" << ";" << endl;
	_file_header.close();

	ofstream _file;
	_file.open("color_table.cpp", ios::app);
	_file << endl << endl << "ColorTable " << name() << "_color_table" << ";" << endl << endl;
	_file << "void fill_" << name() << "_color_table()" << endl << "{" << endl;

	int pixel_count = 32 * 32;

	for (int i = 0; i < mTileCount; i++)
	{
		int r = 0, g = 0, b = 0, a = 0;

		for (int y = 0; y < 32; y++)
		{
			for (int x = 0; x < 32; x++)
			{
				Color& c = mTileSetImage.pixelColor(x, (i * 32) + y);

				r += c.red();
				g += c.green();
				b += c.blue();
			}
		}

		r /= pixel_count;
		g /= pixel_count;
		b /= pixel_count;

		_file << "\t" << name() << "_color_table.push_back(NAS2D::Color{" << r << ", " << g << ", " << b << "});" << endl;

	}

	_file << "}" << endl;
	_file.close();

}
*/
