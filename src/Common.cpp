#include "Common.h"

#include "NAS2D/NAS2D.h"

#include <sstream>

void flipBool(bool& b)
{
	b = !b;
}

/**
 * Draws a beveled box.
 */
void bevelBox(int x, int y, int w, int h, bool sunk, const Color_4ub& bgcolor)
{
	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(x, y, w, h, bgcolor.red(), bgcolor.green(), bgcolor.blue());

	if (!sunk)
	{
		r.drawLine(x, y, x + w, y, COLOR_WHITE);
		r.drawLine(x, y, x, y + h, COLOR_WHITE);
		r.drawLine(x - 1, y + h, x + w, y + h, COLOR_GREY);
		r.drawLine(x + w, y, x + w, y + h, COLOR_GREY);
	}
	else
	{
		r.drawLine(x, y, x + w, y, COLOR_GREY);
		r.drawLine(x, y, x, y + h, COLOR_GREY);
		r.drawLine(x - 1, y + h, x + w, y + h, COLOR_WHITE);
		r.drawLine(x + w, y, x + w, y + h, COLOR_WHITE);
	}
}


int stringToInt(const std::string& s)
{
	int retInt = 0;

	from_string<int>(retInt, s, std::dec);
	
	return retInt;
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


void BlendPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
	Uint8 bR = 0, bG = 0, bB = 0, bA = 0;
	get_pixel32(srf, x, y, bR, bG, bB, bA);

	int outR = (R * A) + (bR * (1.0 - A));
	int outG = (G * A) + (bG * (1.0 - A));
	int outB = (B * A) + (bB * (1.0 - A));
	int outA = (A * A) + (bA * (1.0 - A));

	//outR = srcR * srcA + dstR(1 - srcA);

	Uint32 color = SDL_MapRGBA(srf->format, outR, outG, outB, outA);

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
