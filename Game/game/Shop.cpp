#include "Game.h"
#include "headers/Shop.h"
#include "headers/Player.h"
#include "headers/UIDialogBox.h"

Shop::Shop(float w, float h)
{
	//upgrade Amoun

	damageUpgrade = 1;
	fireRateUpgrade = 3;
	energyUpgrade = 5;
	energyRecoveryUpgrade = 2;
	armorUpgrade = 10;
	armorRepairRateUpgrade = 3;
	dashUpgrade = 1;

 


	Width = w;
	Height = h;
	font.LoadDefault();

	//shop
	testRobot.LoadModel("shop/shop2.md3");
	testRobot.LoadTexture("shop/shop.png");
	testRobot.SetScale(45.5f);
	testRobot.SetPosition(1500, 0, 400);
	testRobot.SetRotation(0, -100, 20);
	testRobot.AddAnimation("idle", 0, 0);
	testRobot.AddAnimation("active", 1, 54);
 

	//shop
	shopImg.LoadImage("shop/shopBg.jpg");
	shopImg.SetSize((float)w, (float)h);
	shopImg.SetPosition((float)w / 2, (float)h / 2);


 
 
	//pages
	shopEnergyPage.LoadImage("shop/shopEnergyPage.png");
	shopEnergyPage.SetSize((float)w, (float)h);
	shopEnergyPage.SetPosition((float)w / 2, (float)h / 2);

	shopDamagePage.LoadImage("shop/shopDamagePage.png");
	shopDamagePage.SetSize((float)w, (float)h);
	shopDamagePage.SetPosition((float)w / 2, (float)h / 2);

	shopFireRateePage.LoadImage("shop/shopFireRateePage.png");
	shopFireRateePage.SetSize((float)w, (float)h);
	shopFireRateePage.SetPosition((float)w / 2, (float)h / 2);

	shopEnergyRecovPage.LoadImage("shop/shopEnergyRecovPage.png");
	shopEnergyRecovPage.SetSize((float)w, (float)h);
	shopEnergyRecovPage.SetPosition((float)w / 2, (float)h / 2);

	pointer.LoadImage("shop/pointer.png");
	pointer.SetSize(25,25);
 


	sectionTwoArmor.LoadImage("shop/sectionTwoArmor.png");
	sectionTwoArmor.SetSize((float)w, (float)h);
	sectionTwoArmor.SetPosition((float)w / 2, (float)h / 2);

	sectionTwoDash.LoadImage("shop/sectionTwoDash.png");
	sectionTwoDash.SetSize((float)w, (float)h);
	sectionTwoDash.SetPosition((float)w / 2, (float)h / 2);



	upgradeBtn.LoadImage("shop/upgradeBtn.png");
	upgradeBtn.SetSize(200, 60);
	upgradeBtn.SetPosition(1070, Height - 865);

 

	exitShopBtn.LoadImage("shop/shopCancelBtn.png");
	exitShopBtn.SetSize(40, 40);
	exitShopBtn.SetPosition(Width - 373, Height - 170);

	shopCurrentPage = 0;
	shopCurrentSection = 0;

	totalPagesCountSectionOne = 3; // starting from 0
	totalPagesCountSectionTwo = 1; // section 2  starting from 0
	totalSectionsCount = 1; // starting from 0
}

void Shop::init(float w, float h)
{
	shopIsInRange = false;

}
 
void Shop::OnUpdate(Uint32 t, Player& player, UIDialogBox& dialogBox)
{
	localPlayer = &player;
	testRobot.Update(t);

	CVector displ = localPlayer->playerModel.GetPositionV() - testRobot.GetPositionV();
	float distance = hypot(displ.GetX(), displ.GetZ());

	if (distance < 300)
	{
		shopIsInRange = true;
		testRobot.PlayAnimation("active", 30, false);
		if(!dialogBox.isBoxShowen) dialogBox.showBox(1 ,12,12, 0);
	}
	else
	{
		 shopIsInRange = false;
		 testRobot.PlayAnimation("idle", 30, false);
		 if (dialogBox.isBoxShowen && dialogBox.currentPriority == 0) dialogBox.hideBox();
	}

}

void Shop::OnDraw(CGraphics* g)
{

}

void Shop::OnRender3D(CGraphics* g)
{
	testRobot.Draw(g);
}

void Shop::openShop(CGraphics* g)
{
	isPlayerShoping = true;
	shopImg.Draw(g);




	if(shopCurrentSection == 0)
	{
		switch (shopCurrentPage)
		{
		case 0:
			shopDamagePage.Draw(g);
			pointer.SetPosition(410, Height - 430);
			font.DrawNumber(Width - 826, Height - 515, damageUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, localPlayer->playerDamage, CColor::White(), 36);
			break;
		case 1:
			shopFireRateePage.Draw(g);
			pointer.SetPosition(410, Height - 490);
			font.DrawNumber(Width - 826, Height - 515, fireRateUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, localPlayer->shootingDelay, CColor::White(), 36);
			break;
		case 2:
			shopEnergyPage.Draw(g);
			pointer.SetPosition(410, Height - 550);
			font.DrawNumber(Width - 826, Height - 515, energyUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, localPlayer->maxEnergy, CColor::White(), 36);
			break;
		case 3:
			shopEnergyRecovPage.Draw(g);
			pointer.SetPosition(410, Height - 610);
			font.DrawNumber(Width - 826, Height - 515, energyRecoveryUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, localPlayer->energyRegenPerSec, CColor::White(), 36);
			break;
		default:
			break;
		}
	}
	else if (shopCurrentSection == 1)
	{
		switch (shopCurrentPage)
		{
		case 0:
			sectionTwoArmor.Draw(g);
			pointer.SetPosition(410, Height - 430);
			font.DrawNumber(Width - 826, Height - 515, armorUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, localPlayer->playerMaxArmor, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 730, armorRepairRateUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 825, localPlayer->armorRegenPerSec, CColor::White(), 36);
			break;
		case 1:
			sectionTwoDash.Draw(g);
			pointer.SetPosition(410, Height - 490);
			font.DrawNumber(Width - 826, Height - 515, dashUpgrade, CColor::White(), 36);
			font.DrawNumber(Width - 826, Height - 640, 1, CColor::White(), 36);
			break;
		default:
			break;
		}
	}
	
	pointer.Draw(g);
	exitShopBtn.Draw(g);
	upgradeBtn.Draw(g);
 

	//resource 1
	font.DrawNumber(Width - 70, Height - 55, localPlayer->armorComponents, localPlayer->armorComponents > 0 ? CColor::Green() : CColor::Red(), 36);

	//resource 2
	font.DrawNumber(Width - 70, Height - 115, localPlayer->weaponComponents, localPlayer->weaponComponents > 0 ? CColor::Green() : CColor::Red(), 36);

	//resource 3
	font.DrawNumber(Width - 70, Height - 175, localPlayer->bossLoot, localPlayer->bossLoot > 0 ? CColor::Green() : CColor::Red(), 36);
}


void Shop::OnKeyDown(SDLKey sym)
{
	if (sym == SDLK_s || sym == SDLK_UP) shopCurrentPage++;
	if (sym == SDLK_w || sym == SDLK_DOWN) shopCurrentPage--;

	if (shopCurrentSection == 0)
	{
		if (shopCurrentPage > totalPagesCountSectionOne) shopCurrentPage = 0;
		if (shopCurrentPage < 0) shopCurrentPage = totalPagesCountSectionOne;
	}
	else if (shopCurrentSection == 1)
	{
		if (shopCurrentPage > totalPagesCountSectionTwo) shopCurrentPage = 0;
		if (shopCurrentPage < 0) shopCurrentPage = totalPagesCountSectionTwo;
	}



	if (sym == SDLK_a || sym == SDLK_LEFT)
	{
		shopCurrentPage = 0;
		shopCurrentSection++;
	}
	if (sym == SDLK_d || sym == SDLK_RIGHT)
	{
		shopCurrentPage = 0;
		shopCurrentSection--;
	}
	if (shopCurrentSection > totalSectionsCount) shopCurrentSection = 0;
	if (shopCurrentSection < 0) shopCurrentSection = totalSectionsCount;
}

void Shop::OnLButtonDown(float x, float y)
{
	if (upgradeBtn.HitTest(x, y))
	{
		if (shopCurrentSection == 0 && localPlayer->weaponComponents > 0)
		{
			switch (shopCurrentPage)
			{
			case 0:
				localPlayer->weaponComponents--;
				localPlayer->playerDamage += damageUpgrade;
				break;
			case 1:
				localPlayer->weaponComponents--;
				localPlayer->shootingDelay -= fireRateUpgrade;
				break;
			case 2:
				localPlayer->weaponComponents--;
				localPlayer->maxEnergy += energyUpgrade;
				break;
			case 3:
				localPlayer->weaponComponents--;
				localPlayer->energyRegenPerSec += energyRecoveryUpgrade;
				break;
			default:
				break;
			}
		}
		else if (shopCurrentSection == 1 && localPlayer->armorComponents > 0 )
		{
			switch (shopCurrentPage)
			{
			case 0:
				localPlayer->armorComponents--;
				localPlayer->playerMaxArmor += armorUpgrade;
				localPlayer->armorRegenPerSec += armorRepairRateUpgrade;

				break;
			case 1:
				//DASH
				localPlayer->armorComponents--;
				//localPlayer->energyRegenPerSec += dashUpgrade;
				break;
			default:
				break;
			}
		}




		/*if (localPlayer->armorComponents > 0)
		{
			localPlayer->armorComponents--;
			localPlayer->playerMaxArmor++;
		}*/
			
	}

 

	if (exitShopBtn.HitTest(x, y))
	{
		isPlayerShoping = false;
		shopCurrentPage = 0;
	}

}