#pragma once

#include "UIContainer.h"


class Window : public UIContainer
{
public:
	Window();
	virtual ~Window();

	bool dragging() const { return mDragging; }

	virtual bool responding_to_events() const { return dragging(); }

	void titleFont(Font& font);
	int titleBarHeight() const;

	virtual void update();

protected:
	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y) final;
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y) final;
	virtual void onMouseMotion(int x, int y, int dX, int dY) final;

	virtual void draw() = 0;	/**< Derived types must override this. */

	virtual void mouseDown(NAS2D::EventHandler::MouseButton button, int x, int y) {};
	virtual void mouseUp(NAS2D::EventHandler::MouseButton button, int x, int y) {};
	virtual void mouseMotion(int x, int y, int relX, int relY) {};

	Point<int>& _mouseCoords() { return mMouseCoords; }	/**< Internal function for derived types. */

private:
	Window(const Window&) = delete;				/**< Not allowed */
	Window& operator=(const Window&) = delete;	/**< Not allowed */

private:
	Font*		mBoldFont = nullptr;			/**< Font used for window title. */

	Point<int>	mMouseCoords;					/**<  */

	bool		mDragging = false;				/**< Window is being dragged. */
};