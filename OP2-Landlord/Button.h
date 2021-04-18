#pragma once

#include "NAS2D/NAS2D.h"


#include "Control.h"

#include <string>

using namespace NAS2D;

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

	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y);
	virtual void onMouseMotion(int x, int y, int dX, int dY);

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

	Image*			mImage;

	bool			mMouseHover;		/**< Mouse is within the bounds of the Button. */
};
