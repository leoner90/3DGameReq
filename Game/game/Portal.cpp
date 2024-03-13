#include "Game.h"
#include "headers/Portal.h"

Portal::Portal(int gameWidth, int gameHeigth)
{
	localH = gameHeigth;
	localW = gameWidth;

	//PORTAL
	portal.LoadModel("portal/portal.obj");
	portal.SetScale(12.f);
	portal.SetPosition(400, 0, 300);
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

	portalIcon.LoadImage("portalIcon.png");
	portalIcon.SetSize(20, 20);
	portalIcon.SetPosition(25, gameHeigth - 105);
}

void Portal::init()
{
	TotaltimeForPortalToCharg = 60000 * 1;
	currentportalChargingTime = 0;

	rechargeTime = 30000; //30 sec recharge if got too many damage
	portalRechargeTimer = 0;
	
	portalUnderAttack = false;
	portalHpBar.SetHealth(0); //UI
 
}

void Portal::OnUpdate(Uint32 t)
{
	CColor currentPortalColor = CColor::Blue();
	if (portalUnderAttack) currentPortalColor = CColor::DarkRed();
	portalHpBar.SetColor(currentPortalColor);

	if (!portalUnderAttack) currentportalChargingTime = t;

	float portalChargeInPercent = currentportalChargingTime / 60000.f * 100.f;


	cout << portalChargeInPercent << endl;
	if ((int)portalChargeInPercent > portalHpBar.GetHealth())
		portalHpBar.SetHealth((int)portalChargeInPercent);
}

void Portal::OnDraw(CGraphics* g)
{
	portalHpBar.Draw(g);
	portalIcon.Draw(g);
}

void Portal::OnRender3D(CGraphics* g)
{
	portal.Draw(g);
	portalPartOne.Draw(g);
	portalPartTwo.Draw(g);
	portalPartThree.Draw(g);
	portalPartFour.Draw(g);
}
