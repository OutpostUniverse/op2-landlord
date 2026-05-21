/**
 * \file	Slider.c++
 * \brief	Functions for sliding controls.
 * \author	Goof
 */
#include "Slider.h"

#include <NAS2D/EnumMouseButton.h>

#include <algorithm>

using namespace NAS2D;


/**
 * C'tor
 */
Slider::Slider() : Control()
{
	Utility<EventHandler>::get().mouseButtonDown().connect({this, &Slider::onMouseDown});
	Utility<EventHandler>::get().mouseButtonUp().connect({this, &Slider::onMouseUp});
	Utility<EventHandler>::get().mouseMotion().connect({this, &Slider::onMouseMotion});
	hasFocus(true);

	mButton1.press().connect({this, &Slider::button1_Pressed});
	mButton2.press().connect({this, &Slider::button2_Pressed});
}


/**
 * D'tor
 */
Slider::~Slider()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect({this, &Slider::onMouseDown});
	Utility<EventHandler>::get().mouseButtonUp().disconnect({this, &Slider::onMouseUp});
	Utility<EventHandler>::get().mouseMotion().disconnect({this, &Slider::onMouseMotion});

	mButton1.press().disconnect({this, &Slider::button1_Pressed});
	mButton2.press().disconnect({this, &Slider::button2_Pressed});
}


/**
 *
 */
void Slider::size(float w, float h)
{
	Control::size(w, h);

	// deduce the type of slider from the ratio.
	if (rect().size.y > rect().size.x) { mSliderType = SLIDER_VERTICAL; }
	else { mSliderType = SLIDER_HORIZONTAL; }

	logic();
}


void Slider::positionChanged(float dX, float dY)
{
	mSlideBar.position.x = mSlideBar.position.x + dX;
	mSlideBar.position.y = mSlideBar.position.y + dY;

	mSlider.position.x = mSlider.position.x + dX;
	mSlider.position.y = mSlider.position.y + dY;

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
		mButton1.position(rect().position.x, rect().position.y);
		mButton1.size(rect().size.x, rect().size.x);
		mButton1.image("sys/up.png");

		mButton2.position(rect().position.x, rect().position.y + rect().size.y - rect().size.x);
		mButton2.size(rect().size.x, rect().size.x);
		mButton2.image("sys/down.png");

		mSlideBar.position.x = rect().position.x;
		mSlideBar.position.y = rect().position.y + rect().size.x;
		mSlideBar.size.x = rect().size.x;
		mSlideBar.size.y = rect().size.y - 2 * rect().size.x;
	}
	else
	{
		mButton1.position(rect().position.x, rect().position.y);
		mButton1.size(rect().size.y, rect().size.y);
		mButton1.image("sys/left.png");

		mButton2.position(rect().position.x + rect().size.x - rect().size.y, rect().position.y);
		mButton2.size(rect().size.y, rect().size.y);
		mButton2.image("sys/right.png");

		mSlideBar.position.x = rect().position.x + rect().size.y;
		mSlideBar.position.y = rect().position.y;
		mSlideBar.size.x = rect().size.x - 2 * rect().size.y;
		mSlideBar.size.y = rect().size.y;
	}
}


/*!
 * Get internal slider position.
 */
float Slider::positionInternal()
{
	return mPosition;
}


/**
 *  \brief set internal slider position
 */
void Slider::positionInternal(float _pos)
{
	mPosition = std::clamp(_pos, 0.0f, mLength);
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
void Slider::onMouseDown(MouseButton button, NAS2D::Point<int> position)
{
	if (!enabled() || !visible() || !hasFocus()) { return; }
	if (button != MouseButton::Left) { return; }

	if (pointInRect_f(position.x, position.y, mSlider))
	{
		mThumbPressed = true;
		mMouseHoverSlide = true;
	}
}


/**
 *
 */
void Slider::onMouseUp(MouseButton button, NAS2D::Point<int> position)
{
	mThumbPressed = false;
	mMouseHoverSlide = false;

	if (button != MouseButton::Left) { return; }
	if (!enabled() || !visible() || !hasFocus()) { return; }

	if (pointInRect_f(position.x, position.y, mSlider))
	{
		// nothing
	}
	else if (pointInRect_f(position.x, position.y, mSlideBar))
	{
		if (mSliderType == SLIDER_VERTICAL)
		{
			if (position.y < mSlider.position.y) { changeThumbPosition(-3.0); }
			else { changeThumbPosition(+3.0); }
		}
		else
		{
			if (position.x < mSlider.position.x) { changeThumbPosition(-3.0); }
			else { changeThumbPosition(+3.0); }
		}
	}
}


/**
 *
 */
void Slider::onMouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change)
{
	if (!enabled() || !visible() || !hasFocus()) { return; }
	if (!mThumbPressed) { return; }

	if (!pointInRect_f(position.x, position.y, rect()) && !mMouseHoverSlide) { return; }

	mMouseX = position.x;
	mMouseY = position.y;

	if (mSliderType == SLIDER_VERTICAL)
	{
		if (position.y < mSlideBar.position.y || position.y >(mSlideBar.position.y + mSlideBar.size.y))
		{
			return;
		}

		positionInternal(mLength * ((position.y - mSlideBar.position.y) / mSlideBar.size.y));
		mCallback(thumbPosition());
	}
	else
	{
		if (position.x < mSlideBar.position.x || position.x >(mSlideBar.position.x + mSlideBar.size.x))
		{
			return;
		}

		positionInternal(mLength * (position.x - mSlideBar.position.x) / mSlideBar.size.x);
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

	r.drawBoxFilled({mSlideBar.position.x - 0.5f, mSlideBar.position.y, mSlideBar.size.x, mSlideBar.size.y}, NAS2D::Color{100, 100, 100});
	r.drawBox({mSlideBar.position.x - 0.5f, mSlideBar.position.y, mSlideBar.size.x, mSlideBar.size.y}, NAS2D::Color{50, 50, 50});

	mButton1.update();
	mButton2.update();

	if (mButton1Held || mButton2Held)
	{
		if (mTimer.elapsedTicks() >= mPressedAccumulator)
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
		mSlider.size.x = mSlideBar.size.x; // height = slide bar height
		mSlider.size.y = static_cast<int>(mSlideBar.size.y / mLength); //relative width
		if (mSlider.size.y < mSlider.size.x) // not too relative. Minimum = Height itself
		{
			mSlider.size.y = mSlider.size.x;
		}

		const auto thumbPosition = ((mSlideBar.size.y - mSlider.size.y) * mPosition) / mLength; //relative width

		mSlider.position.x = mSlideBar.position.x;
		mSlider.position.y = mSlideBar.position.y + thumbPosition;
	}
	else
	{
		// Slider
		mSlider.size.y = mSlideBar.size.y;	// height = slide bar height
		mSlider.size.x = static_cast<int>(mSlideBar.size.x / (mLength + 1)); //relative width
		
		if (mSlider.size.x < mSlider.size.y)	// not too relative. Minimum = Heigt itself
		{
			mSlider.size.x = mSlider.size.y;
		}

		const auto thumbPosition = ((mSlideBar.size.x - mSlider.size.x) * mPosition) / mLength; //relative width

		mSlider.position.x = mSlideBar.position.x + thumbPosition;
		mSlider.position.y = mSlideBar.position.y;
	}

	const auto intRect = mSlider.to<int>();
	bevelBox(intRect.position.x, intRect.position.y, intRect.size.x, intRect.size.y);


	if (fontSet() && mDisplayPosition && mMouseHoverSlide)
	{
		textHover = std::to_string(static_cast<int>(thumbPosition())) + " / " + std::to_string(static_cast<int>(mLength));
		_w = font().width(textHover) + 4;
		_h = font().height() + 4;

		if (mSliderType == SLIDER_VERTICAL)
		{
			_x = static_cast<int>(mSlideBar.position.x + mSlideBar.size.x + 2);
			_y = mMouseY - _h;
		}
		else
		{
			_x = mMouseX + 2;
			_y = static_cast<int>(mSlideBar.position.y) - 2 - _h;
		}

		r.drawBox(NAS2D::Rectangle{_x - _w / 2, _y, _w, _h}, NAS2D::Color{255, 255, 255, 180});
		r.drawBoxFilled(NAS2D::Rectangle{_x + 1 - _w / 2, _y + 1, _w - 2, _h - 2}, NAS2D::Color{0, 0, 0, 180});
		r.drawText(font(), textHover, NAS2D::Point{_x + 2 - _w / 2, _y + 2}, NAS2D::Color{220, 220, 220});
	}
}


/**
 * Set the current value
 */
void Slider::thumbPosition(float value)
{
	if (mBackward) { value = mLength - value; }

	positionInternal(value);

	mCallback(thumbPosition());
}


/**
* Gets the current value of position
*/
float Slider::thumbPosition()
{
	float value = mPosition;
	if (mBackward) { value = mLength - value; }

	return value;
}


/**
 * Adds the change amount to the current position.
 *
 * \param	change	Amount to change in percent to change the
 *					slider's position. Must be between 0.0 1.0.
 */
void Slider::changeThumbPosition(float change)
{
	positionInternal(mPosition + change);
	mCallback(thumbPosition());
}


/**
 * Returns the max value position can get
 */
float Slider::length()
{
	return mLength;
}


/**
 * Set the max value position can get
 */
void Slider::length(float newLength)
{
	mLength = newLength;
}
