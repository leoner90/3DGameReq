#include "Game.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/UIDialogBox.h"


PlayerInterface::PlayerInterface(float gameWidth, float gameHeigth)
{
	localH = gameHeigth;
	localW = gameWidth;
	font.LoadDefault();

	// setup healthbar
	hbar.SetSize(140, 15);
	hbar.SetPosition(115, gameHeigth - 30.f);
	hbar.SetColors(CColor::Red(), CColor::Black(), CColor::Black());

	//Armor Bar
	armorBar.SetSize(140, 15);
	armorBar.SetPosition(115, gameHeigth - 55.f);
	armorBar.SetColors(CColor::Gray(), CColor::Black(), CColor::Black());

	//energyBar
	energyBar.SetSize(140, 15);
	energyBar.SetPosition(115, gameHeigth - 80.f); 
	energyBar.SetColors(CColor::Yellow(), CColor::Black(), CColor::Black());

	//icons
	healtIcon.LoadImage("healthIcon.png");
	healtIcon.SetSize(20, 20);
	healtIcon.SetPosition(25, gameHeigth - 30.f);

	armorIcon.LoadImage("armorIcon.png");
	armorIcon.SetSize(20, 20);
	armorIcon.SetPosition(25, gameHeigth - 55.f);

	energyIcon.LoadImage("energyIcon.png");
	energyIcon.SetSize(20, 20);
	energyIcon.SetPosition(25, gameHeigth - 80.f);

	//Resources Icons
	armorComponent.LoadImage("armorComponent.png");
	armorComponent.SetSize(25, 30);
	armorComponent.SetPosition(localW - 170, localH - 30);

	weaponComponent.LoadImage("weaponComponent.png");
	weaponComponent.SetSize(30, 30);
	weaponComponent.SetPosition(localW - 75, localH - 30);

	skillBarY = 400;
	//skillBar
	skillBar.LoadImage("skillBar.png");
	skillBar.SetSize(120, 120);
	skillBar.SetPosition(localW - 100, skillBarY);

	sprintSkillLogo.LoadImage("sprintSkillLogo.png");
	sprintSkillLogo.SetSize(120, 120);
	sprintSkillLogo.SetPosition(localW - 105, skillBarY - 5);

	callbackSkillIcon.LoadImage("callbackSkillIcon.png");
	callbackSkillIcon.SetSize(120, 120);
	callbackSkillIcon.SetPosition(localW - 105, skillBarY - 5);
}

void PlayerInterface::init(int gameWidth, int gameHeigth)
{
	hbar.SetHealth(100);
	armorBar.SetHealth(100);
	energyBar.SetHealth(100);
}

void PlayerInterface::OnUpdate(Uint32 t, Player& player, UIDialogBox& dialogBox)
{

	/*
	if (dialogBox.isBoxShowen)
	{
		//skillbar attached to the dialogbox
		skillBar.SetPosition(skillBar.GetX(), dialogBox.dialogBoxBg.GetY() + 190);
		sprintSkillLogo.SetPosition(sprintSkillLogo.GetX(), dialogBox.dialogBoxBg.GetY() + 185);
		callbackSkillIcon.SetPosition(callbackSkillIcon.GetX(), dialogBox.dialogBoxBg.GetY() + 185);
	}

	if (dialogBox.dialogBoxBg.GetY() <= 0)
	{
		skillBar.SetPosition(skillBar.GetX(), dialogBox.dialogBoxBg.GetY() + 100);
		sprintSkillLogo.SetPosition(sprintSkillLogo.GetX(), dialogBox.dialogBoxBg.GetY() + 95);
		callbackSkillIcon.SetPosition(callbackSkillIcon.GetX(), dialogBox.dialogBoxBg.GetY() + 95);
	}
	*/

	localPlayer = &player;
	float remainingHpInPercentage = localPlayer->playerCurrentHp / (localPlayer->playerMaxHp / 100);
	float remainingEnergyInPercentage = localPlayer->CurrentEnergy / (localPlayer->maxEnergy / 100);
	float remainingArmorInPercentage = localPlayer->playerCurrentArmor / (localPlayer->playerMaxArmor / 100);

	hbar.SetHealth(remainingHpInPercentage);
	energyBar.SetHealth(remainingEnergyInPercentage);
	armorBar.SetHealth(remainingArmorInPercentage);
}

void PlayerInterface::OnDraw(CGraphics* g)
{
	hbar.Draw(g);
	energyBar.Draw(g);
	armorBar.Draw(g);
	skillBar.Draw(g);
 
	switch (localPlayer->curentSkillSelected)
	{
	case 0:
		callbackSkillIcon.Draw(g);
		break;
	case 1:
		sprintSkillLogo.Draw(g);
		break;
	default:
		break;
	} 
	 
	//icons
	healtIcon.Draw(g);
	armorIcon.Draw(g);
	energyIcon.Draw(g);

	//resources
	armorComponent.Draw(g);
	font.DrawNumber(localW - 145, localH - 40, localPlayer->armorComponents, CColor::White(), 30);

	weaponComponent.Draw(g);
	font.DrawNumber(localW - 50, localH - 40, localPlayer->weaponComponents, CColor::White(), 30);
}