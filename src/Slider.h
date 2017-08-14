/*!
 * \file	Slider.h
 * \brief	Header for sliding controls.
 * \author Goof
 */
#pragma once
#include "Button.h"
#include "Common.h"
#include "Control.h"


 /**
 * \class	Slider
 * \brief	Base class for all sliding controls.
 * 
 * Handle horizontal and vertical slide bar.
 * The handle could go backward or forward.
 * the handle width/height depend on the max value.
 * the minimum width/height is the height/width of the slide bar
 */
class Slider : public Control
{
public:
	typedef NAS2D::Signals::Signal1<double> ValueChangedCallback; /*!< type for Callback on value changed. */

public:
	Slider();
	virtual ~Slider();

	void thumbPosition(double value);		/*!< Set the current position. */
	double thumbPosition();					/*!< Get the current position. */
	void changeThumbPosition(double change);/*!< Adds the change amount to the current position. */

	bool displayPosition() { return mDisplayPosition; }			/*!< Get the position display flag. */
	void displayPosition(bool _d) { mDisplayPosition = _d; }	/*!< Set the position display flag. */

	double length(); 			/*!< Get the max value for the slide area. */
	void length(double _lengh);	/*!< Set the max value for the slide area. */

	bool backward() { return mBackward; }	 	/*!< Get the backward flag. */
	void backward(bool _b) { mBackward = _b; } 	/*!< Set the backward flag. */

	void update(); 							/*!< Called to display the slider. */
	virtual void size(float w, float h); 	/*!< Set the slider size. */

	ValueChangedCallback& change() { return mCallback; } 	/*!< Give the callback to enable another control or a window to dis/connect to this event call. */

protected:
	virtual void onMouseDown(EventHandler::MouseButton button, int x, int y); 	/*!< Event raised on mouse button down. */
	virtual void onMouseUp(EventHandler::MouseButton button, int x, int y); 	/*!< Event raised on mouse button up. */
	virtual void onMouseMotion(int x, int y, int dX, int dY); 	/*!< Event raised on mouse move. */

private:
	/*!
	 * \enum 	SliderType
	 * \brief	List the types of slider that could be used
	 */
	enum SliderType
	{
		SLIDER_VERTICAL,	/*!< Vertical slider. */
		SLIDER_HORIZONTAL	/*!< Horizontal slider. */
	};

private:
	Slider(const Slider&) = delete;				/**< Not allowed */
	Slider& operator=(const Slider&) = delete;	/**< Not allowed */

	double positionInternal();
	void positionInternal(double _pos);

	virtual void positionChanged(float dX, float dY);

	void draw();		/*!< Draw the widget on screen. */
	void logic();		/*!< Compute some values before drawing the control. */

	void button1_Pressed(bool pressed);
	void button2_Pressed(bool pressed);

private:
	Timer					mTimer;

	ValueChangedCallback	mCallback;					/*!< Callback executed when the value is changed. */

	SliderType				mSliderType = SLIDER_VERTICAL;	/*!< Type of the Slider. */

	// mouse event related vars
	int						mMouseX = 0;				/*!< Mouse X coordinate. Used to prevent odd behavior in the thumb. */
	int						mMouseY = 0;				/*!< Mouse X coordinate. Used to prevent odd behavior in the thumb. */

	bool					mMouseHoverSlide = false;	/*!< Mouse is within the bounds of the Button. */
	bool					mThumbPressed = false;		/*!< Flag to indicate if this control is pressed. */

	// Slider button responses
	uint32_t				mPressedAccumulator = 0;	/**< Accumulation value for pressed responses. */
	bool					mButton1Held = false;		/**< Flag indicating that a button is being held down. */
	bool					mButton2Held = false;		/**< Flag indicating that a button is being held down. */

	// Slider values
	double					mPosition = 0;				/*!< Current value that represent the position of the slider. */
	double					mLenght = 0;				/*!< Maximum value for the position of the slider. */

	bool					mBackward = false;			/*!< Does the value returned in backward mode . */

	// drawing vars
	bool					mDisplayPosition = false;	/*!< Indicate if the slider display the value on mouse over. */

	Button					mButton1;
	Button					mButton2;

	Rectangle_2df			mSlideBar;					/*!< Area on screen where the slide area is displayed. */
	Rectangle_2df			mSlider;					/*!< Area on screen where the slider is displayed. */
};
