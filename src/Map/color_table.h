#pragma once

#include <vector>
#include <NAS2D/NAS2D.h>

typedef std::vector<NAS2D::Color_4ub>	ColorTable;

extern ColorTable well0000_color_table;
extern ColorTable well0001_color_table;
extern ColorTable well0002_color_table;
extern ColorTable well0003_color_table;
extern ColorTable well0004_color_table;
extern ColorTable well0005_color_table;
extern ColorTable well0006_color_table;
extern ColorTable well0007_color_table;
extern ColorTable well0008_color_table;
extern ColorTable well0009_color_table;
extern ColorTable well0010_color_table;
extern ColorTable well0011_color_table;
extern ColorTable well0012_color_table;

void fillColorTables();
const ColorTable& selectColorTable(const std::string& tsetName);