#include "Game.h"
#include "headers/loadingScreen.h"

LoadingScreen::LoadingScreen(float w, float h)
{
	localH = h;
	localW = w;

	font.LoadDefault();
	font.Load("arial.ttf");
	text[0] = "Blinky is a droid with an Ai companion created by Yer.";
	text[1] = "Alumni are the titles given to the graduates of The Academy.  ";
	text[2] = "Yor is the husband of Myne. Crafted many things Myne uses on her daily adventures";


	//loadingIcon.PlayAnimation();
	loadingIcon.LoadImage("animationLogoSprite.png", CColor::Black(), 8);
	loadingIcon.SetSize(100, 100);
	loadingIcon.SetPosition(localW - 200, 100);
	loadingIcon.PlayAnimation(0, 7, 8, true);

	// Main Menu screen
	loadingBg.LoadImage("LoadingScreen.jpg");
	loadingBg.SetSize(localW, localH);
	loadingBg.SetPosition(localW / 2, localH / 2);
}


void LoadingScreen::init()
{
	loadingCompleted = false;
	loadingStarted = false;
	randomTextSelectionNum = rand() % 2;
	loadingTimer = 0;
}

void LoadingScreen::Update(Uint32 t)
{
	if (loadingTimer == 0 && loadingStarted) loadingTimer = t + 5000;
	if (loadingTimer < t && loadingStarted) loadingCompleted = true;
	loadingIcon.Update(t);
}

void LoadingScreen::OnDraw(CGraphics* g)
{
	loadingBg.Draw(g);
	font.DrawText(localW - 500, localH / 100 * 60, text[randomTextSelectionNum], CColor::White(), 14);

	//resources
	if (loadingCompleted)
	{
		font.DrawText(localW - 420, 75, "Press Enter To Continue", CColor::White(), 32);
	}
	else
	{
		font.DrawText(localW - 430, 75, "Loading...", CColor::White(), 32);
		loadingIcon.Draw(g);
	}
	
}
