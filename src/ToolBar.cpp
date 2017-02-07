#include "ToolBar.h"


#include "Common.h"


const int BUTTON_SPACE = 2;

ToolBar::ToolBar() : mFont("fonts/ui-normal.png", 7, 9, 0)
{
	initUi();
}


ToolBar::~ToolBar()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().Disconnect(this, &ToolBar::onKeyDown);
	e.mouseWheel().Disconnect(this, &ToolBar::onMouseWheel);

	mToolbarEvent.Clear();
}


void ToolBar::initUi()
{
	// MAP NAME
	txtMapName.position(5, 8);
	txtMapName.width(150);
	txtMapName.font(mFont);

	// SAVE
	btnSave.image("sys/save.png");
	btnSave.size(22, 28);
	btnSave.position(txtMapName.positionX() + txtMapName.width() + 20, 2);
	btnSave.click().Connect(this, &ToolBar::btnSave_Clicked);

	// EDIT TOOLS
	btnPencil.image("sys/pencil.png");
	btnPencil.type(Button::BUTTON_TOGGLE);
	btnPencil.toggle(true);
	btnPencil.size(22, 28);
	btnPencil.position(btnSave.positionX() + btnSave.width() + 18 + BUTTON_SPACE, 2);
	btnPencil.click().Connect(this, &ToolBar::btnPencil_Clicked);

	btnFill.image("sys/paintcan.png");
	btnFill.type(Button::BUTTON_TOGGLE);
	btnFill.size(22, 28);
	btnFill.position(btnPencil.positionX() + btnPencil.width() + BUTTON_SPACE, 2);
	btnFill.click().Connect(this, &ToolBar::btnFill_Clicked);

	btnErase.image("sys/erase.png");
	btnErase.type(Button::BUTTON_TOGGLE);
	btnErase.size(22, 28);
	btnErase.position(btnFill.positionX() + btnFill.width() + BUTTON_SPACE, 2);
	btnErase.click().Connect(this, &ToolBar::btnErase_Clicked);


	// LAYER EDIT
	btnLayerBase.image("sys/layer_b_edit.png");
	btnLayerBase.type(Button::BUTTON_TOGGLE);
	btnLayerBase.toggle(true);
	btnLayerBase.size(22, 28);
	btnLayerBase.position(btnErase.positionX() + btnErase.width() + 18 + BUTTON_SPACE, 2);
	btnLayerBase.click().Connect(this, &ToolBar::btnLayerBase_Clicked);

	btnLayerBaseDetail.image("sys/layer_bd_edit.png");
	btnLayerBaseDetail.type(Button::BUTTON_TOGGLE);
	btnLayerBaseDetail.size(22, 28);
	btnLayerBaseDetail.position(btnLayerBase.positionX() + btnLayerBase.width() + BUTTON_SPACE, 2);
	btnLayerBaseDetail.click().Connect(this, &ToolBar::btnLayerBaseDetail_Clicked);

	btnLayerDetail.image("sys/layer_d_edit.png");
	btnLayerDetail.type(Button::BUTTON_TOGGLE);
	btnLayerDetail.size(22, 28);
	btnLayerDetail.position(btnLayerBaseDetail.positionX() + btnLayerBaseDetail.width() + BUTTON_SPACE, 2);
	btnLayerDetail.click().Connect(this, &ToolBar::btnLayerDetail_Clicked);

	btnLayerForeground.image("sys/layer_fg_edit.png");
	btnLayerForeground.type(Button::BUTTON_TOGGLE);
	btnLayerForeground.size(22, 28);
	btnLayerForeground.position(btnLayerDetail.positionX() + btnLayerDetail.width() + BUTTON_SPACE, 2);
	btnLayerForeground.click().Connect(this, &ToolBar::btnLayerForeground_Clicked);

	btnLayerCollision.image("sys/collision.png");
	btnLayerCollision.type(Button::BUTTON_TOGGLE);
	btnLayerCollision.size(22, 28);
	btnLayerCollision.position(btnLayerForeground.positionX() + btnLayerForeground.width() + BUTTON_SPACE, 2);
	btnLayerCollision.click().Connect(this, &ToolBar::btnLayerCollision_Clicked);

	btnSpinnerUp.image("sys/up.png");
	btnSpinnerUp.size(22, 14);
	btnSpinnerUp.position(btnLayerCollision.positionX() + btnLayerCollision.width() + BUTTON_SPACE + 21, 2);
	btnSpinnerUp.click().Connect(this, &ToolBar::btnSpinnerUp_Clicked);

	btnSpinnerDown.image("sys/down.png");
	btnSpinnerDown.size(22, 13);
	btnSpinnerDown.position(btnLayerCollision.positionX() + btnLayerCollision.width() + BUTTON_SPACE + 21, 17);
	btnSpinnerDown.click().Connect(this, &ToolBar::btnSpinnerDown_Clicked);

	// LAYER VISIBILITY
	btnLayerBaseToggle.image("sys/layer_b_show.png");
	btnLayerBaseToggle.type(Button::BUTTON_TOGGLE);
	btnLayerBaseToggle.toggle(true);
	btnLayerBaseToggle.size(22, 28);
	btnLayerBaseToggle.position(btnSpinnerDown.positionX() + btnSpinnerDown.width() + 18 + BUTTON_SPACE, 2);
	btnLayerBaseToggle.click().Connect(this, &ToolBar::btnLayerBaseToggle_Clicked);

	btnLayerBaseDetailToggle.image("sys/layer_bd_show.png");
	btnLayerBaseDetailToggle.type(Button::BUTTON_TOGGLE);
	btnLayerBaseDetailToggle.toggle(true);
	btnLayerBaseDetailToggle.size(22, 28);
	btnLayerBaseDetailToggle.position(btnLayerBaseToggle.positionX() + btnLayerBaseToggle.width() + BUTTON_SPACE, 2);
	btnLayerBaseDetailToggle.click().Connect(this, &ToolBar::btnLayerBaseDetailToggle_Clicked);

	btnLayerDetailToggle.image("sys/layer_d_show.png");
	btnLayerDetailToggle.type(Button::BUTTON_TOGGLE);
	btnLayerDetailToggle.toggle(true);
	btnLayerDetailToggle.size(22, 28);
	btnLayerDetailToggle.position(btnLayerBaseDetailToggle.positionX() + btnLayerBaseDetailToggle.width() + BUTTON_SPACE, 2);
	btnLayerDetailToggle.click().Connect(this, &ToolBar::btnLayerDetailToggle_Clicked);

	btnLayerForegroundToggle.image("sys/layer_fg_show.png");
	btnLayerForegroundToggle.type(Button::BUTTON_TOGGLE);
	btnLayerForegroundToggle.toggle(true);
	btnLayerForegroundToggle.size(22, 28);
	btnLayerForegroundToggle.position(btnLayerDetailToggle.positionX() + btnLayerDetailToggle.width() + BUTTON_SPACE, 2);
	btnLayerForegroundToggle.click().Connect(this, &ToolBar::btnLayerForegroundToggle_Clicked);

	btnLayerCollisionToggle.image("sys/collision_show.png");
	btnLayerCollisionToggle.type(Button::BUTTON_TOGGLE);
	btnLayerCollisionToggle.toggle(false);
	btnLayerCollisionToggle.size(22, 28);
	btnLayerCollisionToggle.position(btnLayerForegroundToggle.positionX() + btnLayerForegroundToggle.width() + BUTTON_SPACE, 2);
	btnLayerCollisionToggle.click().Connect(this, &ToolBar::btnLayerCollisionToggle_Clicked);

	// WINDOW VISIBILITY
	btnMiniMapToggle.image("sys/map.png");
	btnMiniMapToggle.type(Button::BUTTON_TOGGLE);
	btnMiniMapToggle.toggle(true);
	btnMiniMapToggle.size(22, 28);
	btnMiniMapToggle.position(Utility<Renderer>::get().width() - 48, 2);
	btnMiniMapToggle.click().Connect(this, &ToolBar::btnMiniMapToggle_Clicked);

	btnTilePaletteToggle.image("sys/palette.png");
	btnTilePaletteToggle.type(Button::BUTTON_TOGGLE);
	btnTilePaletteToggle.toggle(true);
	btnTilePaletteToggle.size(22, 28);
	btnTilePaletteToggle.position(Utility<Renderer>::get().width() - 24, 2);
	btnTilePaletteToggle.click().Connect(this, &ToolBar::btnTilePaletteToggle_Clicked);

	hookEvents();
}


void ToolBar::hookEvents()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().Connect(this, &ToolBar::onKeyDown);
	e.mouseWheel().Connect(this, &ToolBar::onMouseWheel);
}


void ToolBar::onKeyDown(KeyCode code, KeyModifier mod, bool repeat)
{
	if (txtMapName.hasFocus() && code == KEY_ENTER || code == KEY_KP_ENTER)
		txtMapName.hasFocus(false);
}


void ToolBar::onMouseWheel(int x, int y)
{
	if (y == 1)
		btnSpinnerUp_Clicked();
	else
		btnSpinnerDown_Clicked();
}


void drawSeparator(Button& btn, int margin)
{
	//bevelBox(btn.positionX() + btn.width() + margin, btn.positionY(), 2, btn.height());
	Renderer& r = Utility<Renderer>::get();

	r.drawLine(btn.positionX() + btn.width() + margin, btn.positionY(), btn.positionX() + btn.width() + margin, btn.positionY() + btn.height(), 130, 130, 130);
	r.drawLine(btn.positionX() + btn.width() + margin + 1, btn.positionY(), btn.positionX() + btn.width() + margin + 1, btn.positionY() + btn.height(), 200, 200, 200);
}


void ToolBar::update()
{
	Renderer& r = Utility<Renderer>::get();
	bevelBox(0, 0, r.width(), 32);

	txtMapName.update();

	btnSave.update();

	drawSeparator(btnSave, 9);

	btnPencil.update();
	btnFill.update();
	btnErase.update();

	drawSeparator(btnErase, 9);

	btnLayerBase.update();
	btnLayerBaseDetail.update();
	btnLayerDetail.update();
	btnLayerForeground.update();
	btnLayerCollision.update();

	r.drawBoxFilled(btnSpinnerUp.rect().x() - 21, btnSpinnerUp.rect().y(), 20, 28, 255, 255, 255);
	r.drawBox(btnSpinnerUp.rect().x() - 21, btnSpinnerUp.rect().y(), 20, 28, 0, 0, 0);
	r.drawText(mFont, string_format("%i", static_cast<int>(mBrush.width())), btnSpinnerUp.rect().x() - 18 + (mFont.width(string_format("%i", static_cast<int>(mBrush.width()))) / 2), 12, 0, 0, 0);

	btnSpinnerUp.update();
	btnSpinnerDown.update();

	drawSeparator(btnLayerBaseToggle, - 32);

	btnLayerBaseToggle.update();
	btnLayerBaseDetailToggle.update();
	btnLayerDetailToggle.update();
	btnLayerForegroundToggle.update();
	btnLayerCollisionToggle.update();

	btnMiniMapToggle.update();
	btnTilePaletteToggle.update();
}


void ToolBar::resetTools()
{
	btnLayerCollisionToggle.toggle(false);
	btnLayerCollisionToggle.enabled(true);
	btnLayerBaseDetailToggle_Clicked();

	btnFill.enabled(true);
	btnErase.enabled(true);
}


void ToolBar::btnLayerBase_Clicked()
{
	btnLayerBase.toggle(true);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);

	resetTools();

	mToolbarEvent(TOOLBAR_LAYER_BG_EDIT);
}


void ToolBar::btnLayerBaseDetail_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(true);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);

	resetTools();

	mToolbarEvent(TOOLBAR_LAYER_BG_DETAIL_EDIT);
}


void ToolBar::btnLayerDetail_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(true);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);

	resetTools();

	mToolbarEvent(TOOLBAR_LAYER_DETAIL_EDIT);
}


void ToolBar::btnLayerForeground_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(true);
	btnLayerCollision.toggle(false);

	resetTools();

	mToolbarEvent(TOOLBAR_LAYER_FOREGROUND_EDIT);
}


void ToolBar::btnLayerCollision_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(true);

	btnLayerCollisionToggle.toggle(true);
	btnLayerCollisionToggle.enabled(false);
	btnLayerBaseDetailToggle_Clicked();

	btnFill.enabled(false);
	btnErase.enabled(false);
	btnPencil_Clicked();

	mToolbarEvent(TOOLBAR_LAYER_COLLISION_EDIT);
}


void ToolBar::btnSave_Clicked()
{
	mToolbarEvent(TOOLBAR_SAVE);
}


void ToolBar::btnPencil_Clicked()
{
	btnPencil.toggle(true);
	btnFill.toggle(false);
	btnErase.toggle(false);

	mToolbarEvent(TOOLBAR_TOOL_PENCIL);
}


void ToolBar::btnFill_Clicked()
{
	btnPencil.toggle(false);
	btnFill.toggle(true);
	btnErase.toggle(false);

	mToolbarEvent(TOOLBAR_TOOL_FILL);
}


void ToolBar::btnErase_Clicked()
{
	btnPencil.toggle(false);
	btnFill.toggle(false);
	btnErase.toggle(true);

	mToolbarEvent(TOOLBAR_TOOL_ERASER);

}


void ToolBar::btnLayerBaseToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_LAYER_BG_TOGGLE);
}


void ToolBar::btnLayerBaseDetailToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_LAYER_BG_DETAIL_TOGGLE);
}


void ToolBar::btnLayerDetailToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_LAYER_DETAIL_TOGGLE);
}


void ToolBar::btnLayerForegroundToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_LAYER_FOREGROUND_TOGGLE);
}


void ToolBar::btnLayerCollisionToggle_Clicked()
{
	mToolbarEvent(TOOLBAR_LAYER_COLLISION_TOGGLE);
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
