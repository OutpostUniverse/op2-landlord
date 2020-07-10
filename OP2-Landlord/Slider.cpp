/**
 * \file	Slider.c++
 * \brief	Functions for sliding controls.
 * \author	Goof
 */
#include "Slider.h"


#include <algorithm>


/**
 * C'tor
 */
Slider::Slider() : Control()
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &Slider::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().connect(this, &Slider::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().connect(this, &Slider::onMouseMotion);
	hasFocus(true);

	mButton1.press().connect(this, &Slider::button1_Pressed);
	mButton2.press().connect(this, &Slider::button2_Pressed);
}


/**
 * D'tor
 */
Slider::~Slider()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &Slider::onMouseDown);
	Utility<EventHandler>::get().mouseButtonUp().disconnect(this, &Slider::onMouseUp);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &Slider::onMouseMotion);

	mButton1.press().disconnect(this, &Slider::button1_Pressed);
	mButton2.press().disconnect(this, &Slider::button2_Pressed);
}


/**
 *
 */
void Slider::size(float w, float h)
{
	Control::size(w, h);

	// deduce the type of slider from the ratio.
	if (rect().height > rect().width) { mSliderType = SLIDER_VERTICAL; }
	else { mSliderType = SLIDER_HORIZONTAL; }

	logic();
}


void Slider::positionChanged(float dX, float dY)
{
	mSlideBar.x = mSlideBar.x + dX;
	mSlideBar.y = mSlideBar.y + dY;

	mSlider.x = mSlider.x + dX;
	mSlider.y = mSlider.y + dY;

	mButton1.position(mButton1.positionX() + dX, mButton1.positionY() + dY);
	mButton2.position(mButton2.positionX() + dX, mButton2.positionY() + dY);
}


/**
 *
 */
void Slider::logic()
{
	// compute position of items
	if (mSliderType == SLIDER_VERTICAL)
	{
		mButton1.position(rect().x, rect().y);
		mButton1.size(rect().width, rect().width);
		mButton1.image("sys/up.png");

		mButton2.position(rect().x, rect().y + rect().height - rect().width);
		mButton2.size(rect().width, rect().width);
		mButton2.image("sys/down.png");

		mSlideBar.x = rect().x;
		mSlideBar.y = rect().y + rect().width;
		mSlideBar.width = rect().width;
		mSlideBar.height = rect().height - 2 * rect().width;
	}
	else
	{
		mButton1.position(rect().x, rect().y);
		mButton1.size(rect().height, rect().height);
		mButton1.image("sys/left.png");

		mButton2.position(rect().x + rect().width - rect().height, rect().y);
		mButton2.size(rect().height, rect().height);
		mButton2.image("sys/right.png");

		mSlideBar.x = rect().x + rect().height;
		mSlideBar.y = rect().y;
		mSlideBar.width = rect().width - 2 * rect().height;
		mSlideBar.height = rect().height;
	}
}


/*!
 * Get internal slider position.
 */
double Slider::positionInternal()
{
	return mPosition;
}


/**
 *  \brief set internal slider position
 */
void Slider::positionInternal(double _pos)
{
	mPosition = std::clamp(_pos, 0.0, mLenght);
}


void Slider::button1_Pressed(bool pressed)
{
	if (pressed) { changeThumbPosition(-1.0); }
	mButton1Held = pressed;

	mTimer.reset();
	mPressedAccumulator = 300;
}


void Slider::button2_Pressed(bool pressed)
{
	if (pressed) { changeThumbPosition(+1.0); }
	mButton2Held = pressed;
	mTimer.reset();
	mPressedAccumulator = 300;
}


/**
 *
 */
void Slider::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!enabled() || !visible() || !hasFocus()) { return; }
	if (button != EventHandler::MouseButton::BUTTON_LEFT) { return; }

	if (pointInRect_f(x, y, mSlider))
	{
		mThumbPressed = true;
		mMouseHoverSlide = true;
	}
}


/**
 *
 */
void Slider::onMouseUp(EventHandler::MouseButton button, int x, int y)
{
	mThumbPressed = false;
	mMouseHoverSlide = false;

	if (button != EventHandler::MouseButton::BUTTON_LEFT) { return; }
	if (!enabled() || !visible() || !hasFocus()) { return; }

	if (pointInRect_f(x, y, mSlider))
	{
		// nothing
	}
	else if (pointInRect_f(x, y, mSlideBar))
	{
		if (mSliderType == SLIDER_VERTICAL)
		{
			if (y < mSlider.y) { changeThumbPosition(-3.0); }
			else { changeThumbPosition(+3.0); }
		}
		else
		{
			if (x < mSlider.x) { changeThumbPosition(-3.0); }
			else { changeThumbPosition(+3.0); }
		}
	}
}


/**
 *
 */
void Slider::onMouseMotion(int x, int y, int dX, int dY)
{
	if (!enabled() || !visible() || !hasFocus()) { return; }
	if (!mThumbPressed) { return; }

	if (!pointInRect_f(x, y, rect()) && !mMouseHoverSlide) { return; }

	mMouseX = x;
	mMouseY = y;

	if (mSliderType == SLIDER_VERTICAL)
	{
		if (y < mSlideBar.y || y >(mSlideBar.y + mSlideBar.height))
		{
			return;
		}

		positionInternal(mLenght * ((y - mSlideBar.y) / mSlideBar.height));
		mCallback(thumbPosition());
	}
	else
	{
		if (x < mSlideBar.x || x >(mSlideBar.x + mSlideBar.width))
		{
			return;
		}

		positionInternal(mLenght * (x - mSlideBar.x) / mSlideBar.width);
		mCallback(thumbPosition());
	}
}


/**
 *
 */
void Slider::update()
{
	draw();
}


/**
 *
 */
void Slider::draw()
{
	if (!visible()) { return; }

	Renderer& r = Utility<Renderer>::get();
	std::string textHover;
	int _x = 0, _y = 0, _w = 0, _h = 0;
	float _thumbPosition = 0.0f;

	r.drawBoxFilled(mSlideBar.x - 0.5f, mSlideBar.y, mSlideBar.width, mSlideBar.height, 100, 100, 100);
	r.drawBox(mSlideBar.x - 0.5f, mSlideBar.y, mSlideBar.width, mSlideBar.height, 50, 50, 50);

	mButton1.update();
	mButton2.update();

	if (mButton1Held || mButton2Held)
	{
		if (mTimer.accumulator() >= mPressedAccumulator)
		{
			mPressedAccumulator = 75;
			mTimer.reset();
			if(mButton1Held) { changeThumbPosition(-1.0); }
			else { changeThumbPosition(1.0); }
		}
	}

	if (mSliderType == SLIDER_VERTICAL)
	{
		// Slider
		mSlider.width = mSlideBar.width; // height = slide bar height
		mSlider.height = static_cast<int>(mSlideBar.height / mLenght); //relative width
		if (mSlider.height < mSlider.width) // not too relative. Minimum = Height itself
		{
			mSlider.height = mSlider.width;
		}

		_thumbPosition = (mSlideBar.height - mSlider.height)  * (mPosition / mLenght); //relative width

		mSlider.x = mSlideBar.x;
		mSlider.y = mSlideBar.y + _thumbPosition;
	}
	else
	{
		// Slider
		mSlider.height = mSlideBar.height;	// height = slide bar height
		mSlider.width = static_cast<int>(mSlideBar.width / (mLenght + 1)); //relative width
		
		if (mSlider.width < mSlider.height)	// not too relative. Minimum = Heigt itself
		{
			mSlider.width = mSlider.height;
		}

		_thumbPosition = (mSlideBar.width - mSlider.width)  * (mPosition / mLenght); //relative width

		mSlider.x = mSlideBar.x + _thumbPosition;
		mSlider.y = mSlideBar.y;
	}

	bevelBox(mSlider.x, mSlider.y, mSlider.width, mSlider.height);


	if (fontSet() && mDisplayPosition && mMouseHoverSlide)
	{
		textHover = std::to_string(static_cast<int>(thumbPosition())) + " / " + std::to_string(static_cast<int>(mLenght));
		_w = font().width(textHover) + 4;
		_h = font().height() + 4;

		if (mSliderType == SLIDER_VERTICAL)
		{
			_x = mSlideBar.x + mSlideBar.width + 2;
			_y = mMouseY - _h;
		}
		else
		{
			_x = mMouseX + 2;
			_y = mSlideBar.y - 2 - _h;
		}

		r.drawBox(_x - _w / 2, _y, _w, _h, 255, 255, 255, 180);
		r.drawBoxFilled(_x + 1 - _w / 2, _y + 1, _w - 2, _h - 2, 0, 0, 0, 180);
		r.drawText(font(), textHover, _x + 2 - _w / 2, _y + 2, 220, 220, 220);
	}
}


/**
 * Set the current value
 */
void Slider::thumbPosition(double value)
{
	if (mBackward) { value = mLenght - value; }

	positionInternal(value);

	mCallback(thumbPosition());
}


/**
* Gets the current value of position
*/
double Slider::thumbPosition()
{
	double value = mPosition;
	if (mBackward) { value = mLenght - value; }

	return value;
}


/**
 * Adds the change amount to the current position.
 *
 * \param	change	Amount to change in percent to change the
 *					slider's position. Must be between 0.0 1.0.
 */
void Slider::changeThumbPosition(double change)
{
	positionInternal(mPosition + change);
	mCallback(thumbPosition());
}


/**
 * Returns the max value position can get
 */
double Slider::length()
{
	return mLenght;
}


/**
 * Set the max value position can get
 */
void Slider::length(double _lenght)
{
	mLenght = _lenght;
}
