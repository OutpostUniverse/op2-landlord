#include "Button.h"

#include "Common.h"

using namespace std;

Button::Button():	mState(STATE_NORMAL),
					mType(BUTTON_NORMAL),
					mMouseHover(false)
{
	Utility<EventHandler>::get().mouseButtonDown().Connect(this, &Button::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().Connect(this, &Button::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().Connect(this, &Button::onMouseMotion);
	hasFocus(true);
}


Button::~Button()
{
	Utility<EventHandler>::get().mouseButtonDown().Disconnect(this, &Button::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().Disconnect(this, &Button::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().Disconnect(this, &Button::onMouseMotion);
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


void Button::onMouseDown(MouseButton button, int x, int y)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == BUTTON_LEFT)
	{
		Point_2d click(x, y);


		if(isPointInRect(click, rect()))
		{
			if(mType == BUTTON_NORMAL)
			{
				mState = STATE_PRESSED;
			}
			else
			{
				mState == STATE_PRESSED ? mState = STATE_NORMAL : mState = STATE_PRESSED;
				mCallback();
			}
		}
	}
}


void Button::onMouseUp(MouseButton button, int x, int y)
{
	if(!enabled() || !visible() || !hasFocus())
		return;

	if(button == BUTTON_LEFT)
	{
		Point_2d click(x, y);
		
		if(mType == BUTTON_NORMAL)
		{
			mState = STATE_NORMAL;

			if(isPointInRect(click, rect()))
				mCallback();
		}
	}
}


void Button::onMouseMotion(int x, int y, int dX, int dY)
{
	if (isPointInRect(x, y, rect().x(), rect().y(), rect().w(), rect().h()))
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
	if(!visible())
		return;

	Renderer& r = Utility<Renderer>::get();

	if (mState == STATE_NORMAL)
	{
		bevelBox(rect().x(), rect().y(), rect().w(), rect().h(), false);
	}
	else //(mState == STATE_PRESSED)
	{
		bevelBox(rect().x(), rect().y(), rect().w(), rect().h(), true);
	}


	if (fontSet() && !text().empty())
	{
		int x = static_cast<int>(rect().x() + (rect().w() / 2) - (font().width(text()) / 2));
		int y = static_cast<int>(rect().y() + (rect().h() / 2) - (font().height() / 2));

		if (mState == STATE_PRESSED) r.drawText(font(), text(), x + 1, y + 1, 0, 0, 0);
		else r.drawText(font(), text(), x, y, 0, 0, 0);
	}

	if (!enabled())
		r.drawBoxFilled(rect(), 125, 125, 125, 100);
}
