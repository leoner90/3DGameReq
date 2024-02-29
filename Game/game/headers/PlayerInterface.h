#pragma once

class Player;

class PlayerInterface
{
public:
	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate(int portalHealth, Player& player);
	void OnDraw(CGraphics* g);
	CHealthBar hbar,portalHpBar;

private:
	Player* localPlayer;
};

 