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
	btnSave.position(200, 2);

	// LAYER EDIT
	btnLayerBase.image("sys/layer_b_edit.png");
	btnLayerBase.type(Button::BUTTON_TOGGLE);
	btnLayerBase.toggle(true);
	btnLayerBase.size(22, 28);
	btnLayerBase.position(btnSave.positionX() + btnSave.width() + 18 + BUTTON_SPACE, 2);
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

	// LAYER VISIBILITY
	btnLayerBaseToggle.image("sys/layer_b_show.png");
	btnLayerBaseToggle.type(Button::BUTTON_TOGGLE);
	btnLayerBaseToggle.toggle(true);
	btnLayerBaseToggle.size(22, 28);
	btnLayerBaseToggle.position(btnLayerCollision.positionX() + btnLayerCollision.width() + 18 + BUTTON_SPACE, 2);

	btnLayerBaseDetailToggle.image("sys/layer_bd_show.png");
	btnLayerBaseDetailToggle.type(Button::BUTTON_TOGGLE);
	btnLayerBaseDetailToggle.toggle(true);
	btnLayerBaseDetailToggle.size(22, 28);
	btnLayerBaseDetailToggle.position(btnLayerBaseToggle.positionX() + btnLayerBaseToggle.width() + BUTTON_SPACE, 2);

	btnLayerDetailToggle.image("sys/layer_d_show.png");
	btnLayerDetailToggle.type(Button::BUTTON_TOGGLE);
	btnLayerDetailToggle.toggle(true);
	btnLayerDetailToggle.size(22, 28);
	btnLayerDetailToggle.position(btnLayerBaseDetailToggle.positionX() + btnLayerBaseDetailToggle.width() + BUTTON_SPACE, 2);

	btnLayerForegroundToggle.image("sys/layer_fg_show.png");
	btnLayerForegroundToggle.type(Button::BUTTON_TOGGLE);
	btnLayerForegroundToggle.toggle(true);
	btnLayerForegroundToggle.size(22, 28);
	btnLayerForegroundToggle.position(btnLayerDetailToggle.positionX() + btnLayerDetailToggle.width() + BUTTON_SPACE, 2);

	btnLayerCollisionToggle.image("sys/collision_show.png");
	btnLayerCollisionToggle.type(Button::BUTTON_TOGGLE);
	btnLayerCollisionToggle.toggle(false);
	btnLayerCollisionToggle.size(22, 28);
	btnLayerCollisionToggle.position(btnLayerForegroundToggle.positionX() + btnLayerForegroundToggle.width() + BUTTON_SPACE, 2);

	hookEvents();
}


void ToolBar::hookEvents()
{
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().Connect(this, &ToolBar::onKeyDown);
}


void ToolBar::onKeyDown(KeyCode code, KeyModifier mod, bool repeat)
{
	if (txtMapName.hasFocus() && code == KEY_ENTER || code == KEY_KP_ENTER)
		txtMapName.hasFocus(false);
}


void ToolBar::update()
{
	Renderer& r = Utility<Renderer>::get();
	bevelBox(0, 0, r.width(), 32);

	txtMapName.update();

	btnSave.update();

	btnLayerBase.update();
	btnLayerBaseDetail.update();
	btnLayerDetail.update();
	btnLayerForeground.update();
	btnLayerCollision.update();

	btnLayerBaseToggle.update();
	btnLayerBaseDetailToggle.update();
	btnLayerDetailToggle.update();
	btnLayerForegroundToggle.update();
	btnLayerCollisionToggle.update();
}


void ToolBar::btnLayerBase_Clicked()
{
	btnLayerBase.toggle(true);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);
}


void ToolBar::btnLayerBaseDetail_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(true);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);
}


void ToolBar::btnLayerDetail_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(true);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(false);
}


void ToolBar::btnLayerForeground_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(true);
	btnLayerCollision.toggle(false);
}


void ToolBar::btnLayerCollision_Clicked()
{
	btnLayerBase.toggle(false);
	btnLayerBaseDetail.toggle(false);
	btnLayerDetail.toggle(false);
	btnLayerForeground.toggle(false);
	btnLayerCollision.toggle(true);
}
