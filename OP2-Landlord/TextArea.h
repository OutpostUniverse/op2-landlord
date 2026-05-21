#pragma once

#include "NAS2D/NAS2D.h"

#include "Control.h"

#include <string>
#include <vector>


class TextArea : public Control
{
public:

	typedef NAS2D::Signal<> ClickCallback;

	TextArea();
	virtual ~TextArea();

	void update();

private:
	virtual void onSizeChanged();
	virtual void onTextChanged();
	virtual void onFontChanged();

	virtual void draw();

	std::vector<std::string> split(const char *str, char delim = ' ');
	void processString();

private:
	std::vector<std::string>	mFormattedList;

	size_t		mNumLines;
};
