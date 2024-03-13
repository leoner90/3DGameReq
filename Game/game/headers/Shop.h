#pragma once

class Player;
class UIDialogBox;

class Shop
{
public:
	Shop(float w, float h);
	//~Shop();

	void init(float w, float h);
	void OnUpdate(Uint32 t, Player& player, UIDialogBox& dialogBox);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);
	void openShop(CGraphics* g);
	void OnKeyDown(SDLKey sym);
	void OnLButtonDown(float x, float y);
	bool shopIsInRange, isPlayerShoping;
private:
	CFont font;
	Player* localPlayer;
	CModelMd3 testRobot;
	CSprite shopImg, upgradeWeaponBtn, upgradeArmorBtn, exitShopBtn;

	float Width, Height;



};

 