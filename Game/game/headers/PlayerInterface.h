#pragma once

class Player;
class UIDialogBox;

class PlayerInterface
{
public:
	PlayerInterface(float gameWidth, float gameHeigth);

	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate(Uint32 t, Player& player, UIDialogBox& dialogBox);
	void OnDraw(CGraphics* g);


	CFont font;
	
	CHealthBar hbar, energyBar, armorBar;

	CSprite skillBar, sprintSkillLogoOn, sprintSkillLogoOff, callbackSkillIcon, mainEnergyUI;
private:
	Player* localPlayer;
	float localH, localW;
	CSprite weaponComponent, armorComponent, bossComponent;

	float skillBarY;
	float localTime;
};

 