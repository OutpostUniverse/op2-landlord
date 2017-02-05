#pragma once

#include "NAS2D/NAS2D.h"

using namespace NAS2D;

#include <string>

void flipBool(bool& b);

void bevelBox(int x, int y, int w, int h, bool sunk = false, const Color_4ub& bgcolor = Color_4ub(180, 180, 180, 255));

int stringToInt(const std::string& s);

std::string TrimString(const std::string& src, const std::string& c = " \r\n");

void DrawPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);
void BlendPixel(SDL_Surface *srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);
