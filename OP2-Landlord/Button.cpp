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


void Button::onMouseDown(EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == EventHandler::MouseButton::Left)
	{
		if(rect().contains(position))
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


void Button::onMouseUp(EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == EventHandler::MouseButton::Left)
	{
		if(mType == BUTTON_NORMAL)
		{
			mState = STATE_NORMAL;

			mPressed(false);

			if (rect().contains(position))
			{
				mCallback();
			}
		}
	}
}


void Button::onMouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change)
{
	if (rect().contains(position))
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
		bevelBox(rect().x, rect().y, rect().width, rect().height, false);
	}
	else //(mState == STATE_PRESSED)
	{
		bevelBox(rect().x, rect().y, rect().width, rect().height, true);
	}


	if (mImage)
	{
		int x = static_cast<int>(rect().x + (rect().width / 2) - ((mImage->size().x / 2) + 1));
		int y = static_cast<int>(rect().y + (rect().height / 2) - (mImage->size().y / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawImage(*mImage, NAS2D::Point{x, y});
	}
	else if (fontSet() && !text().empty())
	{
		int x = static_cast<int>(rect().x + (rect().width / 2) - (font().width(text()) / 2));
		int y = static_cast<int>(rect().y + (rect().height / 2) - (font().height() / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawText(font(), text(), NAS2D::Point{x, y}, NAS2D::Color::Black);
	}

	if (!enabled()) { r.drawBoxFilled(rect(), NAS2D::Color{125, 125, 125, 100}); }
}
