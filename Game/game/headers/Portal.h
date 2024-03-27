#pragma once
class UIDialogBox;

class  Portal
{
public:
	 Portal(float gameWidth, float gameHeigth);
	//~ Portal();
	
	 //main Functions
	 void init();
	 void OnUpdate(Uint32 t, UIDialogBox& dialogBox);
	 void OnDraw(CGraphics* g);
	 void OnRender3D(CGraphics* g);

	 //Portal Damaged
	 void portalGettingDamaged(float damageAmount);

	 //To DO getters Setters
	 bool isPortalCharged;
	 bool portalUnderAttack, isPortalReseting;
	 float portalChargeInPercent;
	 CModel portal, portalPartOne, portalPartTwo, portalPartThree, portalPartFour;

private:
	CFont font;
	CSprite  portaChargelUI;
	UIDialogBox* localDialogBox;
	CHealthBar portalHpBar, resetHpBar;

	float localH, localW, localTime;
	float underAttackMsgDelay;
	float portalMaxEnergyField;
	float portalChargePerSec;
	float TotaltimeForPortalToCharg;
	float rechargeTime;
	float portalRechargeTimer;
	float portalEnergyField; // how many attack portal can handel before going to recharge
	float portalResetTimer;
	float damageDelay; // charge delay after hit
	float prevFrameTime, deltatime;
	float currentportalChargingTime;
};

 