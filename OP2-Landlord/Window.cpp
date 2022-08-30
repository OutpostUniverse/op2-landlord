#include "Window.h"

#include <NAS2D/NAS2D.h>

using namespace NAS2D;

const int TITLE_BAR_HEIGHT = 20;


Window::Window()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().connect({this, &Window::onMouseDown});
	e.mouseButtonUp().connect({this, &Window::onMouseUp});
	e.mouseMotion().connect({this, &Window::onMouseMotion});
}


Window::~Window()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().disconnect({this, &Window::onMouseDown});
	e.mouseButtonUp().disconnect({this, &Window::onMouseUp});
	e.mouseMotion().disconnect({this, &Window::onMouseMotion});
}


void Window::titleFont(const Font& font)
{
	mTitleFont = &font;
}


int Window::titleBarHeight() const
{
	return TITLE_BAR_HEIGHT;
}


/**
 * Mouse Button Down event handler.
 */
void Window::onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	if (!visible()) { return; }
	
	mouseDown(button, position);

	if((button != EventHandler::MouseButton::Left)) { return; }

	const auto windowBounds = rect().to<int>();
	const auto titleBarBounds = NAS2D::Rectangle{windowBounds.x, windowBounds.y, windowBounds.width, titleBarHeight()};
	if (titleBarBounds.contains(position))
	{
		mIsWindowDragging = true;
		return;
	}

	if (!(rect().contains(mMouseCoords)))
	{
		return;
	}
}


void Window::onMouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	mIsWindowDragging = false;

	if (!visible()) { return; }
	
	mouseUp(button, position);

	if((button != EventHandler::MouseButton::Left)) { return; }
}


void Window::onMouseMotion(NAS2D::Point<int> mousePosition, NAS2D::Vector<int> change)
{
	mMouseCoords = mousePosition;

	if (!visible()) { return; }

	mouseMotion(mousePosition, change);

	if (mIsWindowDragging)
	{
		position(positionX() + change.x, positionY() + change.y);
		return;
	}
}


void Window::update()
{
	if (!visible()) { return; }

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), NAS2D::Color{180, 180, 180});
	r.drawBoxFilled({rect().x, rect().y, rect().width, TITLE_BAR_HEIGHT}, NAS2D::Color{75, 95, 130});
	r.drawBox(rect(), NAS2D::Color::Black);

	if (mTitleFont)
	{
		r.drawText(*mTitleFont, text(), {rect().x + (rect().width / 2) - (mTitleFont->width(text()) / 2), rect().y + 2});
	}

	draw();

	UIContainer::update();
}