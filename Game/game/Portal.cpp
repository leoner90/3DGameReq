#include "Game.h"
#include "headers/Portal.h"
#include "headers/UIDialogBox.h"

Portal::Portal(float gameWidth, float gameHeigth)
{
	localH = gameHeigth;
	localW = gameWidth;

	//PORTAL
	portal.LoadModel("portal/portal.obj");
	portal.SetScale(230.f);
	portal.SetPosition(400, 80, 300);
	portal.SetHealth(0);

	portalPartOne.LoadModel("portal/portalPartTestobj.obj");
	portalPartOne.SetScale(75.f);
	portalPartOne.SetPosition(800, 0, 300);

	portalPartTwo.LoadModel("portal/portalPartTestobj.obj");
	portalPartTwo.SetScale(75.f);
	portalPartTwo.SetPosition(0, 0, 300);

	portalPartThree.LoadModel("portal/portalPartTestobj.obj");
	portalPartThree.SetScale(75.f);
	portalPartThree.SetPosition(400, 0, 700);

	portalPartFour.LoadModel("portal/portalPartTestobj.obj");
	portalPartFour.SetScale(75.f);
	portalPartFour.SetPosition(400, 0, -100);

	//portal
	portalHpBar.SetSize(140, 15);
	portalHpBar.SetPosition(115, gameHeigth - 105);
	portalHpBar.SetColors(CColor::DarkBlue(), CColor::Black(), CColor::Black());

	//portal
	resetHpBar.SetSize(140, 5);
	resetHpBar.SetPosition(115, gameHeigth - 113);
	resetHpBar.SetColors(CColor::Red(), CColor::Black(), CColor::Black());


	portalIcon.LoadImage("portalIcon.png");
	portalIcon.SetSize(20, 20);
	portalIcon.SetPosition(25, gameHeigth - 105);

	isPortalCharged = false;
}

void Portal::init()
{
	TotaltimeForPortalToCharg = 60000 * 5; // min
	portalChargePerSec = 1000; //

	rechargeTime = 30000; //30 sec recharge if got too many damage
	portalRechargeTimer = 0;
	
	
	portalHpBar.SetHealth(0); //UI
	resetHpBar.SetHealth(0);
	prevFrameTime = 0;
	deltatime = 0;
	currentportalChargingTime = 0;
	isPortalCharged = false;

	portalEnergyField = portalMaxEnergyField = 500;
	portalUnderAttack = false;
	portalResetTimer = 0;
	isPortalReseting = false;
	damageDelay = 0;
	font.LoadDefault();
	portalChargeInPercent = 0;
}

void Portal::OnUpdate(Uint32 t, UIDialogBox& dialogBox)
{
	localTime = t;
	localDialogBox = &dialogBox;

	//getDeltaTime
	if(prevFrameTime != 0)  deltatime = (t - prevFrameTime) / 1000; //in sec
	prevFrameTime = t;

	//if not reseting and last attack was 3 sec ago
	if (damageDelay < localTime && !isPortalReseting)
	{
		portalUnderAttack = false;
	}

	if (portalResetTimer <= localTime && portalResetTimer !=0)
	{
		isPortalReseting = false;
		localDialogBox->showBox(1, 21, 21, 1, 2000);
		portalResetTimer = 0;
	}

	float energyFieldResetPerSec = 20;
	if (portalEnergyField < portalMaxEnergyField) portalEnergyField += deltatime * energyFieldResetPerSec;

	CColor currentPortalColor = CColor::Blue();
	if (portalUnderAttack) currentPortalColor = CColor::DarkRed();
	portalHpBar.SetColor(currentPortalColor);

	//prevFrameTime != 0???
	if (!portalUnderAttack ) currentportalChargingTime += (float)deltatime * portalChargePerSec;
	portalChargeInPercent = currentportalChargingTime / TotaltimeForPortalToCharg * 100.f;

	//float to integer - so not really accurate
	portalHpBar.SetHealth((int)portalChargeInPercent);

	resetHpBar.SetHealth(100 - (portalEnergyField / portalMaxEnergyField * 100));

	portalHpBar.Update(t);
	resetHpBar.Update(t);
	if ((int)portalChargeInPercent >= 100) isPortalCharged = true;

}

void Portal::OnDraw(CGraphics* g)
{
	portalHpBar.Draw(g);
	resetHpBar.Draw(g);
	portalIcon.Draw(g);
	if (isPortalReseting)
	{
		font.DrawNumber(190, localH - 113, (portalResetTimer - localTime) / 1000, CColor::Red(), 18);
	}
	
}

void Portal::OnRender3D(CGraphics* g)
{
	portal.Draw(g);
	portalPartOne.Draw(g);
	portalPartTwo.Draw(g);
	portalPartThree.Draw(g);
	portalPartFour.Draw(g);
}

void Portal::portalGettingDamaged(float damageAmount)
{
	if (isPortalReseting) return;
	portalUnderAttack = true;
	portalEnergyField -= damageAmount;
	damageDelay = 3000 + localTime;
	if (underAttackMsgDelay <= localTime)
	{
		underAttackMsgDelay = 10000 + localTime;
		localDialogBox->showBox(1, 10, 10, 1, 3000);
	}

	if (portalEnergyField <= 0)
	{
		isPortalReseting = true;
		localDialogBox->showBox(1, 11, 11, 1, 2000);
		portalResetTimer = 30000 + localTime; //30 sec rest time
	}
}
