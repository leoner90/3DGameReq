#pragma once

class Player;
class UIDialogBox;

class PlayerInterface
{
public:
	//Main Functions
	void init(int gameWidth, int gameHeigth);
	void OnUpdate(Uint32 t, int portalHealth, Player& player, UIDialogBox& dialogBox);
	void OnDraw(CGraphics* g);
	CFont font;
	
	CHealthBar hbar, portalHpBar, energyBar, armorBar;

	CSprite healtIcon, armorIcon, energyIcon, skillBar, sprintSkillLogo, callbackSkillIcon;
private:
	Player* localPlayer;
	float localH, localW;
	CSprite weaponComponent, armorComponent, portalIcon;

	float skillBarY;
};

 