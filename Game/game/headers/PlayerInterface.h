#pragma once

class Player;

class PlayerInterface
{
public:
	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate(int portalHealth, Player& player);
	void OnDraw(CGraphics* g);
	CFont font;

	CHealthBar hbar, portalHpBar, energyBar, armorBar;

	CSprite healtIcon, armorIcon, energyIcon;
private:
	Player* localPlayer;
};

 