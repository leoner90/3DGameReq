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

	CSprite healtIcon, armorIcon, energyIcon, skillBar, sprintSkillLogo, callbackSkillIcon;
private:
	Player* localPlayer;
	float localH, localW;
	CSprite weaponComponent, armorComponent;

	float skillBarY;
};

 