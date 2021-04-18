#pragma once

#include "NAS2D/NAS2D.h"

#include "Control.h"

#include <string>

using namespace NAS2D;

class TextArea : public Control
{
public:

	typedef NAS2D::Signal<> ClickCallback;

	TextArea();
	virtual ~TextArea();

	void update();

protected:

private:
	typedef std::vector<std::string> StringList;

private:
	virtual void onSizeChanged();
	virtual void onTextChanged();
	virtual void onFontChanged();

	virtual void draw();

	StringList split(const char *str, char delim = ' ');
	void processString();

private:
	StringList	mFormattedList;

	size_t		mNumLines;
};
