#pragma once

class Player;
class UIDialogBox;

class Shop
{
public:
	Shop(float w, float h);
	//~Shop();

	void init(float w, float h);
	void OnUpdate(Uint32 t, Player& player, UIDialogBox& dialogBox);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);
	void openShop(CGraphics* g);
	void OnKeyDown(SDLKey sym);
	void OnLButtonDown(float x, float y);
	bool shopIsInRange, isPlayerShoping;
	CModelMd3 testRobot;
private:
	CFont font;
	Player* localPlayer;
	
	CSprite upgradeBtn, exitShopBtn, pointer;
	CSprite shopImg, shopEnergyPage, shopDamagePage, shopFireRateePage, shopEnergyRecovPage;
	CSprite sectionTwoArmor, sectionTwoDash;
	float Width, Height;

	int shopCurrentPage;
	int shopCurrentSection;

	int totalPagesCountSectionOne , totalPagesCountSectionTwo;
	int totalSectionsCount;


	float damageUpgrade, fireRateUpgrade, energyUpgrade, energyRecoveryUpgrade, armorUpgrade, armorRepairRateUpgrade, dashUpgrade;
};

 