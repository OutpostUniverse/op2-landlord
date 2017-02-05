#include "Button.h"

#include "Common.h"

using namespace std;

Button::Button():	mState(STATE_NORMAL),
					mType(BUTTON_NORMAL),
					mMouseHover(false),
					mImage(nullptr)
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

	if (mImage)
		delete mImage;
}


void Button::image(const std::string path)
{
	if (!Utility<Filesystem>::get().exists(path))
	{
		cout << "Button::image(): specified image file doesn't exist." << endl;
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


	if (mImage)
	{
		int x = static_cast<int>(rect().x() + (rect().w() / 2) - ((mImage->width() / 2) + 1));
		int y = static_cast<int>(rect().y() + (rect().h() / 2) - (mImage->height() / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawImage(*mImage, x, y);
	}
	else if (fontSet() && !text().empty())
	{
		int x = static_cast<int>(rect().x() + (rect().w() / 2) - (font().width(text()) / 2));
		int y = static_cast<int>(rect().y() + (rect().h() / 2) - (font().height() / 2));

		if (mState == STATE_PRESSED) { ++x; ++y; }
		r.drawText(font(), text(), x, y, 0, 0, 0);
	}

	if (!enabled())
		r.drawBoxFilled(rect(), 125, 125, 125, 100);
}
