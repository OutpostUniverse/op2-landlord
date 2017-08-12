#include "ToolBar.h"

#include "Common.h"

const int BUTTON_SPACE = 2;

using namespace NAS2D;

ToolBar::ToolBar() : mFont("fonts/opensans.ttf", 12), mToggle("sys/square.png")
{
	initUi();
}


ToolBar::~ToolBar()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().disconnect(this, &ToolBar::onKeyDown);
	e.mouseWheel().disconnect(this, &ToolBar::onMouseWheel);

	mToolbarEvent.clear();
}


void ToolBar::initUi()
{
	// SAVE
	btnSave.image("sys/save.png");
	btnSave.size(22, 28);
	btnSave.position(2, 2);
	btnSave.click().connect(this, &ToolBar::btnSave_Clicked);

	// EDIT TOOLS
	btnPencil.image("sys/pencil.png");
	btnPencil.type(Button::BUTTON_TOGGLE);
	btnPencil.toggle(true);
	btnPencil.size(22, 28);
	btnPencil.position(btnSave.positionX() + btnSave.width() + 18 + BUTTON_SPACE, 2);
	btnPencil.click().connect(this, &ToolBar::btnPencil_Clicked);

	btnFill.image("sys/paintcan.png");
	btnFill.type(Button::BUTTON_TOGGLE);
	btnFill.size(22, 28);
	btnFill.position(btnPencil.positionX() + btnPencil.width() + BUTTON_SPACE, 2);
	btnFill.click().connect(this, &ToolBar::btnFill_Clicked);

	btnFillContiguous.type(Button::BUTTON_TOGGLE);
	btnFillContiguous.toggle(true);
	btnFillContiguous.size(16, 16);
	btnFillContiguous.position(btnFill.positionX() - 30, 40);
	btnFillContiguous.visible(false);

	mFloodFillExtendedArea(btnFillContiguous.positionX() - 4, btnFillContiguous.positionY() - 4, 104, btnFillContiguous.height() + 8);

	btnErase.image("sys/erase.png");
	btnErase.type(Button::BUTTON_TOGGLE);
	btnErase.size(22, 28);
	btnErase.position(btnFill.positionX() + btnFill.width() + BUTTON_SPACE, 2);
	btnErase.click().connect(this, &ToolBar::btnErase_Clicked);

	// Spinner buttons
	btnSpinnerUp.image("sys/up.png");
	btnSpinnerUp.size(22, 14);
	btnSpinnerUp.position(btnErase.positionX() + btnErase.width() + BUTTON_SPACE + 21, 2);
	btnSpinnerUp.click().connect(this, &ToolBar::btnSpinnerUp_Clicked);

	btnSpinnerDown.image("sys/down.png");
	btnSpinnerDown.size(22, 13);
	btnSpinnerDown.position(btnErase.positionX() + btnErase.width() + BUTTON_SPACE + 21, 17);
	btnSpinnerDown.click().connect(this, &ToolBar::btnSpinnerDown_Clicked);


	// LAYER EDIT
	btnLayerCollision.image("sys/collision.png");
	btnLayerCollision.type(Button::BUTTON_TOGGLE);
	btnLayerCollision.size(22, 28);
	btnLayerCollision.position(btnSpinnerDown.positionX() + 43, 2);


	// WINDOW VISIBILITY
	btnMiniMapToggle.image("sys/map.png");
	btnMiniMapToggle.type(Button::BUTTON_TOGGLE);
	btnMiniMapToggle.toggle(false);
	btnMiniMapToggle.size(22, 28);
	btnMiniMapToggle.position(Utility<Renderer>::get().width() - 48, 2);
	btnMiniMapToggle.click().connect(this, &ToolBar::btnMiniMapToggle_Clicked);

	btnTilePaletteToggle.image("sys/palette.png");
	btnTilePaletteToggle.type(Button::BUTTON_TOGGLE);
	btnTilePaletteToggle.toggle(false);
	btnTilePaletteToggle.size(22, 28);
	btnTilePaletteToggle.position(Utility<Renderer>::get().width() - 24, 2);
	btnTilePaletteToggle.click().connect(this, &ToolBar::btnTilePaletteToggle_Clicked);

	hookEvents();
}


int ToolBar::height()
{
	return 32;
}


void ToolBar::hookEvents()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().connect(this, &ToolBar::onKeyDown);
	e.mouseWheel().connect(this, &ToolBar::onMouseWheel);
	e.windowResized().connect(this, &ToolBar::onWindowResized);
}


void ToolBar::onKeyDown(EventHandler::KeyCode code, EventHandler::KeyModifier mod, bool repeat)
{}


void ToolBar::onMouseWheel(int x, int y)
{
	if (y == 1) { btnSpinnerUp_Clicked(); }
	else { btnSpinnerDown_Clicked(); }
}


void ToolBar::onWindowResized(int width, int height)
{
	btnMiniMapToggle.position(Utility<Renderer>::get().width() - 48, 2);
	btnTilePaletteToggle.position(Utility<Renderer>::get().width() - 24, 2);
}


void drawSeparator(Button& btn, int margin)
{
	Renderer& r = Utility<Renderer>::get();

	r.drawLine(btn.positionX() + btn.width() + margin, btn.positionY(), btn.positionX() + btn.width() + margin, btn.positionY() + btn.height(), 130, 130, 130);
	r.drawLine(btn.positionX() + btn.width() + margin + 1, btn.positionY(), btn.positionX() + btn.width() + margin + 1, btn.positionY() + btn.height(), 200, 200, 200);
}


void ToolBar::update()
{
	Renderer& r = Utility<Renderer>::get();
	bevelBox(0, 0, r.width(), height());

	btnSave.update();

	drawSeparator(btnSave, 9);

	btnPencil.update();
	btnFill.update();
	btnErase.update();

	if (btnFillContiguous.visible())
	{
		r.drawBoxFilled(mFloodFillExtendedArea.x() + 3, mFloodFillExtendedArea.y() + 4, mFloodFillExtendedArea.width(), mFloodFillExtendedArea.height(), 0, 0, 0, 100);
		r.drawBoxFilled(mFloodFillExtendedArea, 180, 180, 180);
		r.drawBox(mFloodFillExtendedArea, 0, 0, 0);
		r.drawText(mFont, "Contiguous", btnFillContiguous.positionX() + btnFillContiguous.width() + 4, btnFillContiguous.positionY() + 4, 0, 0, 0);
		btnFillContiguous.update();
		if (btnFillContiguous.toggled()) r.drawImage(mToggle, btnFillContiguous.positionX() - 1, btnFillContiguous.positionY());
	}

	r.drawBoxFilled(btnSpinnerUp.rect().x() - 21, btnSpinnerUp.rect().y(), 20, 28, 255, 255, 255);
	r.drawBox(btnSpinnerUp.rect().x() - 21, btnSpinnerUp.rect().y(), 20, 28, 0, 0, 0);
	r.drawText(mFont, NAS2D::string_format("%i", static_cast<int>(mBrush.width())), btnSpinnerUp.rect().x() - 18 + (mFont.width(NAS2D::string_format("%i", static_cast<int>(mBrush.width()))) / 2), 12, 0, 0, 0);

	btnSpinnerUp.update();
	btnSpinnerDown.update();

	drawSeparator(btnLayerCollision, -32);
	btnLayerCollision.update();

	btnMiniMapToggle.update();
	btnTilePaletteToggle.update();
}


void ToolBar::resetTools()
{
	btnFill.enabled(true);
	btnErase.enabled(true);
}


void ToolBar::btnSave_Clicked()
{
	mToolbarEvent(TOOLBAR_SAVE);
}


void ToolBar::btnPencil_Clicked()
{
	btnPencil.toggle(true);
	btnFill.toggle(false);
	btnFillContiguous.visible(false);
	btnErase.toggle(false);

	mToolbarEvent(TOOLBAR_TOOL_PENCIL);
}


void ToolBar::btnFill_Clicked()
{
	btnPencil.toggle(false);
	btnFill.toggle(true);
	btnFillContiguous.visible(true);
	btnErase.toggle(false);

	mToolbarEvent(TOOLBAR_TOOL_FILL);
}


void ToolBar::btnErase_Clicked()
{
	btnPencil.toggle(false);
	btnFill.toggle(false);
	btnFillContiguous.visible(false);
	btnErase.toggle(true);

	mToolbarEvent(TOOLBAR_TOOL_ERASER);

}


void ToolBar::btnMiniMapToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_MINIMAP_TOGGLE);
}


void ToolBar::btnTilePaletteToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_TILE_PALETTE_TOGGLE);
}

void ToolBar::btnSpinnerUp_Clicked()
{
	int size = clamp(mBrush.width() + 1, 1, 8);
	mBrush.size(size, size);
}


void ToolBar::btnSpinnerDown_Clicked()
{
	int size = clamp(mBrush.width() - 1, 1, 8);
	mBrush.size(size, size);
}
