#pragma once

class  Portal
{
public:
	 Portal(int gameWidth, int gameHeigth);
	//~ Portal();


	 CModel portal, portalPartOne, portalPartTwo, portalPartThree, portalPartFour;
	 float TotaltimeForPortalToCharg;
	 float currentportalChargingTime;
	 float rechargeTime;
	 float portalRechargeTimer;
	 bool portalUnderAttack;
	 float portalHp; // how many attack portal can handel before going to recharge
	 CHealthBar portalHpBar;

	 void init();
	 void OnUpdate(Uint32 t);
	 void OnDraw(CGraphics* g);
	 void OnRender3D(CGraphics* g);


 
private:
	CSprite  portalIcon;
	float localH, localW;
};

 