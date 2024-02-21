#include "Game.h"
#include "headers/PlayerInterface.h"
 

void PlayerInterface::init(int gameWidth, int gameHeigth)
{
	// setup healthbar
	hbar.SetSize(100, 15);
	hbar.SetPosition(gameWidth - 80, gameHeigth - 30);
	hbar.SetHealth(100);


	portalHpBar.SetSize(100, 15);
	portalHpBar.SetColor(CColor::Blue());
	portalHpBar.SetPosition(80, gameHeigth - 30);
	portalHpBar.SetHealth(100);
	
}

void PlayerInterface::OnUpdate(int portalHealth)
{
	portalHpBar.SetHealth(portalHealth);
}

void PlayerInterface::OnDraw(CGraphics* g)
{
	hbar.Draw(g);
	portalHpBar.Draw(g);
}



