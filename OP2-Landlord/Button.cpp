#include "Button.h"

#include "Common.h"

#include <iostream>

using namespace NAS2D;

Button::Button():	mState(STATE_NORMAL),
					mType(BUTTON_NORMAL),
					mImage(nullptr),
					mMouseHover(false)
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &Button::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().connect(this, &Button::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().connect(this, &Button::onMouseMotion);
	hasFocus(true);
}


Button::~Button()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &Button::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().disconnect(this, &Button::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &Button::onMouseMotion);

	if (mImage)
		delete mImage;
}


void Button::image(const std::string path)
{
	if (!Utility<Filesystem>::get().exists(path))
	{
		std::cout << "Button::image(): specified image file doesn't exist." << std::endl;
		return;
	}

	if (mImage)
	{
		delete mImage;
		mImage = nullptr;
	}

	mImage = new Image(path);
}


void Button::type(Type type)
{
	type ? mType = BUTTON_TOGGLE : mType = BUTTON_NORMAL;
}


void Button::toggle(bool toggle)
{
	toggle ? mState = STATE_PRESSED : mState = STATE_NORMAL;
}


bool Button::toggled() const
{
	if(mState == STATE_PRESSED)
		return true;

	return false;
}


void Button::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == EventHandler::MouseButton::BUTTON_LEFT)
	{
		Point click{x, y};


		if(rect().contains(click))
		{
			if(mType == BUTTON_NORMAL)
			{
				mState = STATE_PRESSED;
				mPressed(true);
			}
			else
			{
				mState == STATE_PRESSED ? mState = STATE_NORMAL : mState = STATE_PRESSED;
				mCallback();
			}
		}
	}
}


void Button::onMouseUp(EventHandler::MouseButton button, int x, int y)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == EventHandler::MouseButton::BUTTON_LEFT)
	{
		Point click{x, y};
		
		if(mType == BUTTON_NORMAL)
		{
			mState = STATE_NORMAL;

			mPressed(false);

			if (rect().contains(click))
			{
				mCallback();
			}
		}
	}
}


void Button::onMouseMotion(int x, int y, int dX, int dY)
{
	if (rect().contains(Point{x, y}))
	{
		mMouseHover = true;
		return;
	}

	mMouseHover = false;
}


void Button::update()
{
	draw();
}

/**
 * Draws the button.
 */
void Button::draw()
{
	if (!visible()) { return; }

	Renderer& r = Utility<Renderer>::get();

	if (mState == STATE_NORMAL)
	{
		bevelBox(rect().x(), rect().y(), rect().width(), rect().height(), false);
	}
	else //(mState == STATE_PRESSED)
	{
		bevelBox(rect().x(), rect().y(), rect().width(), rect().height(), true);
	}


	if (mImage)
	{
		int x = static_cast<int>(rect().x() + (rect().width() / 2) - ((mImage->width() / 2) + 1));
		int y = static_cast<int>(rect().y() + (rect().height() / 2) - (mImage->height() / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawImage(*mImage, x, y);
	}
	else if (fontSet() && !text().empty())
	{
		int x = static_cast<int>(rect().x() + (rect().width() / 2) - (font().width(text()) / 2));
		int y = static_cast<int>(rect().y() + (rect().height() / 2) - (font().height() / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawText(font(), text(), x, y, 0, 0, 0);
	}

	if (!enabled()) { r.drawBoxFilled(rect(), 125, 125, 125, 100); }
}
