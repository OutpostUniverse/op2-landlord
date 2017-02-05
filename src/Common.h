#pragma once

#include "NAS2D/NAS2D.h"

using namespace NAS2D;

#include <string>
#include <memory>

void flipBool(bool& b);

void bevelBox(int x, int y, int w, int h, bool sunk = false, const Color_4ub& bgcolor = Color_4ub(180, 180, 180, 255));

int stringToInt(const std::string& s);

std::string TrimString(const std::string& src, const std::string& c = " \r\n");

void DrawPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);
void BlendPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

/**
* Simple helper function to provide a printf like function.
*/
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	unique_ptr<char[]> buffer(new char[size]);
	snprintf(buffer.get(), size, format.c_str(), args ...);
	return string(buffer.get(), buffer.get() + size - 1);
}