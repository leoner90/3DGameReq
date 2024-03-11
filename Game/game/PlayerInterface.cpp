#include "Game.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/UIDialogBox.h"


void PlayerInterface::init(int gameWidth, int gameHeigth)
{
	localH = gameHeigth;
	localW = gameWidth;
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
	portalHpBar.SetSize(gameWidth, 14);
	portalHpBar.SetPosition(localW / 2, 5);
	portalHpBar.SetColors(CColor::Blue(), CColor::Black(), CColor::Black());
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

	//Resources Icons

	armorComponent.LoadImage("armorComponent.png");
	armorComponent.SetSize(25, 30);
	armorComponent.SetPosition(localW - 170, localH - 30);

	weaponComponent.LoadImage("weaponComponent.png");
	weaponComponent.SetSize(30, 30);
	weaponComponent.SetPosition(localW - 75, localH - 30);

	portalIcon.LoadImage("portalIcon.png");
	portalIcon.SetSize(25, 25);


	skillBarY = 100;
	//skillBar
	skillBar.LoadImage("skillBar.png");
	skillBar.SetSize(120, 120);
	skillBar.SetPosition(localW - 100,  skillBarY);

	sprintSkillLogo.LoadImage("sprintSkillLogo.png");
	sprintSkillLogo.SetSize(120, 120);
	sprintSkillLogo.SetPosition(localW - 105,  skillBarY - 5);

	callbackSkillIcon.LoadImage("callbackSkillIcon.png");
	callbackSkillIcon.SetSize(120, 120);
	callbackSkillIcon.SetPosition(localW - 105,  skillBarY - 5);

 
}

void PlayerInterface::OnUpdate(Uint32 t, int portalHealth, Player& player, UIDialogBox& dialogBox)
{
	if (dialogBox.isBoxShowen)
	{
		portalHpBar.SetPosition(localW / 2, dialogBox.dialogBoxBg.GetY() + 82); //Get Y is center

		//skillbar attached to the dialogbox
		skillBar.SetPosition(skillBar.GetX(), dialogBox.dialogBoxBg.GetY() + 190);
		sprintSkillLogo.SetPosition(sprintSkillLogo.GetX(), dialogBox.dialogBoxBg.GetY() + 185);
		callbackSkillIcon.SetPosition(callbackSkillIcon.GetX(), dialogBox.dialogBoxBg.GetY() + 185);
	}



	if (dialogBox.dialogBoxBg.GetY() <= 0)
	{
		portalHpBar.SetPosition(localW / 2, 5);
		skillBar.SetPosition(skillBar.GetX(), dialogBox.dialogBoxBg.GetY() + 100);
		sprintSkillLogo.SetPosition(sprintSkillLogo.GetX(), dialogBox.dialogBoxBg.GetY() + 95);
		callbackSkillIcon.SetPosition(callbackSkillIcon.GetX(), dialogBox.dialogBoxBg.GetY() + 95);
	}

	portalIcon.SetPosition( 60, portalHpBar.GetY() + 11);
	CColor currentPortalColor = CColor::Blue();
	if (portalHpBar.GetHealth() < 80) currentPortalColor = CColor::Yellow();
	else if (portalHpBar.GetHealth() < 50) currentPortalColor = CColor::Red();
	portalHpBar.SetColor(currentPortalColor);
		
			
 



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
	skillBar.Draw(g);
 
	switch (localPlayer->curentSkillSelected)
	{
	case 0:
		callbackSkillIcon.Draw(g);
		break;
	case 1:
		sprintSkillLogo.Draw(g);
		break;
	} 
	 
	
	
	//icons
	healtIcon.Draw(g);
	armorIcon.Draw(g);
	energyIcon.Draw(g);
	portalIcon.Draw(g);


	//resources

	armorComponent.Draw(g);
	font.DrawNumber(localW - 145, localH - 40, localPlayer->armorComponents, CColor::White(), 30);

	weaponComponent.Draw(g);
	font.DrawNumber(localW - 50, localH - 40, localPlayer->weaponComponents, CColor::White(), 30);

}



