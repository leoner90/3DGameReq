#include "Game.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
 

void PlayerInterface::init(int gameWidth, int gameHeigth)
{
	font.LoadDefault();
	// setup healthbar
	hbar.SetSize(140, 15);
	hbar.SetPosition(115, gameHeigth - 30);
	hbar.SetHealth(100);
	hbar.SetColors(CColor::Red(), CColor::Black(), CColor::Black());

	//Armor Bar
	armorBar.SetSize(140, 15);
	armorBar.SetPosition(115, gameHeigth - 55);
	armorBar.SetHealth(100);
	armorBar.SetColors(CColor::Gray(), CColor::Black(), CColor::Black());

	//energyBar
	energyBar.SetSize(140, 15);
	energyBar.SetPosition(115, gameHeigth - 80);
	energyBar.SetHealth(100);
	energyBar.SetColors(CColor::Yellow(), CColor::Black(), CColor::Black());

	//portal
	portalHpBar.SetSize(140, 25);
	portalHpBar.SetColors(CColor::Blue(), CColor::Black(), CColor::Black());
	portalHpBar.SetPosition(gameWidth - 80, gameHeigth - 30);
	portalHpBar.SetHealth(100);

	//icons
	healtIcon.LoadImage("healthIcon.png");
	healtIcon.SetSize(20,20);
	healtIcon.SetPosition(25 , (float)gameHeigth - 30 );

	armorIcon.LoadImage("armorIcon.png");
	armorIcon.SetSize(20, 20);
	armorIcon.SetPosition(25, (float)gameHeigth - 55);

	energyIcon.LoadImage("energyIcon.png");
	energyIcon.SetSize(20, 20);
	energyIcon.SetPosition(25, (float)gameHeigth - 80);
	
}

void PlayerInterface::OnUpdate(int portalHealth, Player& player)
{
	localPlayer = &player;
	float remainingHpInPercentage = localPlayer->playerCurrentHp / (localPlayer->playerMaxHp / 100);
	float remainingEnergyInPercentage = localPlayer->CurrentEnergy / (localPlayer->maxEnergy / 100);
	float remainingArmorInPercentage = localPlayer->playerCurrentArmor / (localPlayer->playerMaxArmor / 100);

	hbar.SetHealth(remainingHpInPercentage);
	portalHpBar.SetHealth(portalHealth);
	energyBar.SetHealth(remainingEnergyInPercentage);
	armorBar.SetHealth(remainingArmorInPercentage);
}

void PlayerInterface::OnDraw(CGraphics* g)
{
	hbar.Draw(g);
	portalHpBar.Draw(g);
	energyBar.Draw(g);
	armorBar.Draw(g);


	//icons
	healtIcon.Draw(g);
	armorIcon.Draw(g);
	energyIcon.Draw(g);


	//resources
	font.DrawText(1000, 30, "ArmorComponenets :", CColor::White(), 16);
	font.DrawNumber(1200, 30, localPlayer->armorComponents, CColor::White(), 16);

	font.DrawText(1000, 70, "WeaponrComponenets :", CColor::White(), 16);
	font.DrawNumber(1200, 70, localPlayer->weaponComponents, CColor::White(), 16);

}



