#include "Common.h"

#include "Stream/StreamReader.h"

#include "NAS2D/NAS2D.h"

#include <sstream>

void flipBool(bool& b)
{
	b = !b;
}

/**
 * Draws a beveled box.
 */
void bevelBox(int x, int y, int w, int h, bool sunk, const Color& bgcolor)
{
	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(NAS2D::Rectangle{x, y, w, h}, bgcolor);

	if (!sunk)
	{
		r.drawLine(x, y, x + w, y, Color::White);
		r.drawLine(x, y, x, y + h, Color::White);

		r.drawLine(x - 1, y + h - 1, x + w, y + h - 1, Color::Gray);
		r.drawLine(x + w - 1, y + 1, x + w - 1, y + h - 1, Color::Gray);

		r.drawLine(x - 0.5f, y + h, x + w, y + h, Color::Black);
		r.drawLine(x + w, y, x + w, y + h, Color::Black);
	}
	else
	{
		r.drawLine(x, y + h, x + w, y + h, Color::White);
		r.drawLine(x + w, y, x + w, y + h, Color::White);
		
		r.drawLine(x + 1, y + 1, x + w, y + 1, Color::Gray);
		r.drawLine(x + 1, y + 1, x + 1, y + h, Color::Gray);

		r.drawLine(x, y, x + w, y, Color::Black);
		r.drawLine(x, y, x, y + h + 0.5f, Color::Black);
	}
}


std::string TrimString(const std::string& src, const std::string& c)
{
	auto p2 = src.find_last_not_of(c);

	if(p2 == std::string::npos)
		return std::string();

	auto p1 = src.find_first_not_of(c);

	if(p1 == std::string::npos)
		p1 = 0;
	
	return src.substr(p1, (p2 - p1) + 1);
}


int RoundUpPowerOf2(int num)
{
	num = num - 1;
	num = num | (num >> 1);
	num = num | (num >> 2);
	num = num | (num >> 4);
	num = num | (num >> 8);
	num = num | (num >> 16);
	num += 1;

	return num;
}


int LogBase2(int num)
{
	int log = 31;
	int y;

	y = num << 16;  if (y != 0) { log = log - 16;  num = y; }
	y = num << 8;  if (y != 0) { log = log - 8;  num = y; }
	y = num << 4;  if (y != 0) { log = log - 4;  num = y; }
	y = num << 2;  if (y != 0) { log = log - 2;  num = y; }
	y = num << 1;  if (y != 0) { log = log - 1; }

	return log;
}


/**
 * Helper function.
 * 
 * Reads a map tag.
 * 
 * \note throws if tag is incorrect.
 */
void readTag(StreamReader* in, int tag)
{
	int _tag = 0;
	in->read(&_tag, MAP_CHUNK_SIZE);
	if (_tag != tag)
	{
		throw std::runtime_error("MapFile::Load(): tag mismatch");
	}
}


/**
 * Helper function for mouse picking.
 */
int gridLocation(int point, int cameraPoint, int viewportDimension)
{
	return ((point - -(cameraPoint % TILE_SIZE)) / TILE_SIZE) % viewportDimension;
}


/**
 * Convenience function to pass a Rectangle<float> to \c isPointInRect()
 */
bool pointInRect_f(int x, int y, const Rectangle<float>& rect)
{
	return rect.to<int>().contains(NAS2D::Point{x, y});
}


/**
 * Convenience function to pass a \c float's to \c isPointInRect()
 */
bool pointInRect_f(int x, int y, float rectX, float rectY, float rectW, float rectH)
{
	return pointInRect_f(x, y, NAS2D::Rectangle{rectX, rectY, rectW, rectH});
}
