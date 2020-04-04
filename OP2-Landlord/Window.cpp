#include "Window.h"

#include <NAS2D/NAS2D.h>

using namespace NAS2D;

const int TITLE_BAR_HEIGHT = 20;


Window::Window()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().connect(this, &Window::onMouseDown);
	e.mouseButtonUp().connect(this, &Window::onMouseUp);
	e.mouseMotion().connect(this, &Window::onMouseMotion);
}


Window::~Window()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().disconnect(this, &Window::onMouseDown);
	e.mouseButtonUp().disconnect(this, &Window::onMouseUp);
	e.mouseMotion().disconnect(this, &Window::onMouseMotion);
}


void Window::titleFont(Font& font)
{
	mBoldFont = &font;
}


int Window::titleBarHeight() const
{
	return TITLE_BAR_HEIGHT;
}


/**
 * Mouse Button Down event handler.
 */
void Window::onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y)
{
	if (!visible()) { return; }
	
	mouseDown(button, x, y);

	if((button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }

	const auto windowBounds = static_cast<NAS2D::Rectangle<int>>(rect());
	const auto titleBarBounds = NAS2D::Rectangle<int>{windowBounds.x(), windowBounds.y(), windowBounds.width(), titleBarHeight()};
	if (titleBarBounds.contains(NAS2D::Point<int>{x, y}))
	{
		mDragging = true;
		return;
	}

	if (!(rect().contains(mMouseCoords)))
	{
		return;
	}
}


void Window::onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y)
{
	mDragging = false;

	if (!visible()) { return; }
	
	mouseUp(button, x, y);

	if((button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }
}


void Window::onMouseMotion(int x, int y, int dX, int dY)
{
	mMouseCoords = {x, y};

	if (!visible()) { return; }

	mouseMotion(x, y, dX, dY);

	if (mDragging)
	{
		position(positionX() + dX, positionY() + dY);
		return;
	}
}


void Window::update()
{
	if (!visible()) { return; }

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 180, 180, 180);
	r.drawBoxFilled(rect().x(), rect().y(), rect().width(), TITLE_BAR_HEIGHT, 75, 95, 130);
	r.drawBox(rect(), 0, 0, 0);

	if (mBoldFont)
	{
		r.drawText(*mBoldFont, text(), rect().x() + (rect().width() / 2) - (mBoldFont->width(text()) / 2), rect().y() + 2, 255, 255, 255);
	}

	draw();

	UIContainer::update();
}