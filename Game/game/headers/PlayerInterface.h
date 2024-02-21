#pragma once


class PlayerInterface
{
public:
	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate(int portalHealth);
	void OnDraw(CGraphics* g);
	CHealthBar hbar,portalHpBar;

private:
	
};

 