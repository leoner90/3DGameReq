#include "Game.h"
#include "headers/Shop.h"
#include "headers/Player.h"
#include "headers/UIDialogBox.h"

Shop::Shop(float w, float h)
{
	Width = w;
	Height = h;
	font.LoadDefault();

	//shop
	testRobot.LoadModel("shop/testRobot.md3");
	testRobot.LoadTexture("shop/shopTextures.jpg");
	testRobot.SetScale(45.5f);
	testRobot.SetPosition(1500, 0, 400);
	testRobot.AddAnimation("idle", 1, 110);
	testRobot.PlayAnimation("idle", 30, true);

	//shop

	shopImg.LoadImage("shop.jpg");
	shopImg.SetSize((float)w, (float)h);
	shopImg.SetPosition((float)w / 2, (float)h / 2);

	upgradeWeaponBtn.LoadImage("addArmor.jpg");
	upgradeWeaponBtn.SetSize(200, 50);
	upgradeWeaponBtn.SetPosition((float)w / 2 + (w / 100 * 20), (float)h - (h / 100 * 40));

	upgradeArmorBtn.LoadImage("addWeapon.jpg");
	upgradeArmorBtn.SetSize(200, 50);
	upgradeArmorBtn.SetPosition((float)w / 2 + (w / 100 * 20), (float)h - (h / 100 * 60));

	exitShopBtn.LoadImage("shopCancelBtn.png");
	exitShopBtn.SetSize(50, 50);
	exitShopBtn.SetPosition((float)w / 2 + (w / 100 * 42), (float)h - (h / 100 * 24));
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
		if(!dialogBox.isBoxShowen) dialogBox.showBox(1 ,12,12, 0);
	}
	else
	{
		 shopIsInRange = false;
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
	exitShopBtn.Draw(g);

	upgradeWeaponBtn.Draw(g);
	font.DrawText((float)Width / 2 + (Width / 100 * 30), (float)Height - (Height / 100 * 40) , "ARMOR", CColor::White(), 22);
	font.DrawNumber((float)Width / 2 + (Width / 100 * 35), (float)Height - (Height / 100 * 40), localPlayer->playerMaxArmor, CColor::White(), 22);

 
 
	//armor
	upgradeArmorBtn.Draw(g);
	font.DrawText((float)Width / 2 + (Width / 100 * 30), (float)Height - (Height / 100 * 60), "DAMAGE", CColor::White(), 22);
	font.DrawNumber((float)Width / 2 + (Width / 100 * 35), (float)Height - (Height / 100 * 60), localPlayer->playerDamage, CColor::White(), 22);


	font.DrawText((float)Width / 2 - (Width / 100 * 5), (float)Height - (Height / 100 * 24), "Armor Components", CColor::White(), 22);
	font.DrawNumber((float)Width / 2 + (Width / 100 * 10), (float)Height - (Height / 100 * 24), localPlayer->armorComponents, localPlayer->armorComponents > 0 ? CColor::Green() : CColor::Red(), 22);


	font.DrawText((float)Width / 2 + (Width / 100 * 15), (float)Height - (Height / 100 * 24), "Weapon Components", CColor::White(), 22);
	font.DrawNumber((float)Width / 2 + (Width / 100 * 30), (float)Height - (Height / 100 * 24), localPlayer->weaponComponents, localPlayer->weaponComponents > 0 ? CColor::Green() : CColor::Red(), 22);
}


void Shop::OnKeyDown(SDLKey sym)
{
	 
}

void Shop::OnLButtonDown(float x, float y)
{
	if (upgradeWeaponBtn.HitTest(x, y))
	{
		if (localPlayer->armorComponents > 0)
		{
			localPlayer->armorComponents--;
			localPlayer->playerMaxArmor++;
		}
			
	}

	if (upgradeArmorBtn.HitTest(x, y))
	{
		if (localPlayer->weaponComponents > 0)
		{
			localPlayer->weaponComponents--;
			localPlayer->playerDamage++;
		}
		
	}

	if (exitShopBtn.HitTest(x, y))
	{
		isPlayerShoping = false;
	}

}