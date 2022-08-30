#pragma once

#include "NAS2D/NAS2D.h"


#include "Control.h"

#include <string>


class Button: public Control
{
public:
	enum Type
	{
		BUTTON_NORMAL,
		BUTTON_TOGGLE
	};

	typedef NAS2D::Signal<> ClickCallback;
	typedef NAS2D::Signal<bool> PressCallback;

public:
	Button();
	~Button();

	void type(Type type);

	void toggle(bool toggle);
	bool toggled() const;

	ClickCallback& click() { return mCallback; }
	PressCallback& press() { return mPressed; }

	void image(const std::string path);

	void update();

protected:

	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	virtual void onMouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change);

private:

	enum State
	{
		STATE_NORMAL,
		STATE_PRESSED
	};

	void draw();

	State			mState;				/**< Current state of the Button. */
	Type			mType;				/**< Modifies Button behavior. */

	ClickCallback	mCallback;			/**< Signal when the Button is activated. */
	PressCallback	mPressed;			/**< Signal when the Button is pressed (as in click and hold). */

	NAS2D::Image*			mImage;

	bool			mMouseHover;		/**< Mouse is within the bounds of the Button. */
};
