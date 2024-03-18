#pragma once
class UIDialogBox;

class  Portal
{
public:
	 Portal(float gameWidth, float gameHeigth);
	//~ Portal();


	 CModel portal, portalPartOne, portalPartTwo, portalPartThree, portalPartFour;
	 float TotaltimeForPortalToCharg;
	 float rechargeTime;
	 float portalRechargeTimer;
	 bool portalUnderAttack, isPortalReseting;
	 float portalEnergyField; // how many attack portal can handel before going to recharge
	 float portalResetTimer;
	 CHealthBar portalHpBar, resetHpBar;

	 float damageDelay; // dellay after hit

	 void init();
	 void OnUpdate(Uint32 t, UIDialogBox& dialogBox);
	 void OnDraw(CGraphics* g);
	 void OnRender3D(CGraphics* g);
	 void portalGettingDamaged(float damageAmount);

	 float prevFrameTime, deltatime;
	 float currentportalChargingTime;
	 bool isPortalCharged;
private:
	CSprite  portalIcon;
	float localH, localW, localTime;
	UIDialogBox* localDialogBox;
	float underAttackMsgDelay;
	float portalMaxEnergyField;
	CFont font;
	float portalChargePerSec;
};

 