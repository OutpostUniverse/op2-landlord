#pragma once

#include "NAS2D/NAS2D.h"

#include "Control.h"

#include <string>

using namespace NAS2D;

class Button: public Control
{
public:

	typedef Gallant::Signal0<void> ClickCallback;

	enum Type
	{
		BUTTON_NORMAL,
		BUTTON_TOGGLE
	};

	Button();
	~Button();

	void type(Type type);

	void toggle(bool toggle);
	bool toggled() const;

	ClickCallback& click() { return mCallback; }

	void image(const std::string path);

	void update();

protected:

	virtual void onMouseDown(MouseButton button, int x, int y);
	virtual void onMouseUp(MouseButton button, int x, int y);
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

	ClickCallback	mCallback;			/**< Object to notify when the Button is activated. */

	Image*			mImage;

	bool			mMouseHover;		/**< Mouse is within the bounds of the Button. */
};
