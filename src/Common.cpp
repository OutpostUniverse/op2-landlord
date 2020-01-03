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

	r.drawBoxFilled(x, y, w, h, bgcolor.red(), bgcolor.green(), bgcolor.blue());

	if (!sunk)
	{
		r.drawLine(x, y, x + w, y, Color::White);
		r.drawLine(x, y, x, y + h, Color::White);

		r.drawLine(x - 1, y + h - 1, x + w, y + h - 1, Color::Grey);
		r.drawLine(x + w - 1, y + 1, x + w - 1, y + h - 1, Color::Grey);

		r.drawLine(x - 0.5f, y + h, x + w, y + h, Color::Black);
		r.drawLine(x + w, y, x + w, y + h, Color::Black);
	}
	else
	{
		r.drawLine(x, y + h, x + w, y + h, Color::White);
		r.drawLine(x + w, y, x + w, y + h, Color::White);
		
		r.drawLine(x + 1, y + 1, x + w, y + 1, Color::Grey);
		r.drawLine(x + 1, y + 1, x + 1, y + h, Color::Grey);

		r.drawLine(x, y, x + w, y, Color::Black);
		r.drawLine(x, y, x, y + h + 0.5f, Color::Black);
	}
}


std::string TrimString(const std::string& src, const std::string& c)
{
	int p2 = src.find_last_not_of(c);

	if(p2 == std::string::npos)
		return std::string();

	int p1 = src.find_first_not_of(c);

	if(p1 == std::string::npos)
		p1 = 0;
	
	return src.substr(p1, (p2 - p1) + 1);
}


/**
 * Draws a pixel to an SDL_Surface.
 * 
 * Note that this is mostly here as a hack to get Minimaps working
 * until we can build a proper way to draw pixels directly to
 * a texture.
 */
void DrawPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
	Uint32 color = SDL_MapRGBA(srf->format, R, G, B, A);

	if(SDL_MUSTLOCK(srf))
		if(SDL_LockSurface(srf) < 0 )
			return;

	switch(srf->format->BytesPerPixel)
	{
		case 1:		// 8-bpp
			{
			Uint8 *bufp;

			bufp = (Uint8*)srf->pixels + y*srf->pitch + x;
			*bufp = color;
			}
			break;

	case 2: // 15-bpp or 16-bpp
		{
		Uint16 *bufp;

		bufp = (Uint16*)srf->pixels + y*srf->pitch/2 + x;
		*bufp = color;
		}
		break;

	case 3: // 24-bpp
		{
		Uint8 *bufp;

		bufp = (Uint8*)srf->pixels + y * srf->pitch + x;
		*(bufp + srf->format->Rshift / 8) = R;
		*(bufp + srf->format->Gshift / 8) = G;
		*(bufp + srf->format->Bshift / 8) = B;
		*(bufp + srf->format->Ashift / 8) = A;
		}
		break;

	case 4: // Probably 32-bpp
		{
			Uint32 *bufp;

			bufp = (Uint32*)srf->pixels + y * srf->pitch / 4 + x;
			*bufp = color;
		}
		break;
	}

	if(SDL_MUSTLOCK(srf))
		SDL_UnlockSurface(srf);
}


void get_pixel32(SDL_Surface *surface, int x, int y, Uint8 &r, Uint8 &g, Uint8 &b, Uint8 &a)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;

	SDL_GetRGBA(pixels[(y * surface->w) + x], surface->format, &r, &g, &b, &a);
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
 * Convenience function to pass a Rectangle_2df to \c isPointInRect()
 */
bool pointInRect_f(int x, int y, const Rectangle_2df& rect)
{
	return pointInRect_f(x, y, rect.x(), rect.y(), rect.width(), rect.height());
}


/**
 * Convenience function to pass a \c float's to \c isPointInRect()
 */
bool pointInRect_f(int x, int y, float rectX, float rectY, float rectW, float rectH)
{
	return isPointInRect(x, y,	static_cast<int>(rectX), static_cast<int>(rectY), static_cast<int>(rectW), static_cast<int>(rectH));
}
