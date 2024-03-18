#include "Game.h"
#include "headers/loadingScreen.h"
#include "headers/textConverter.h"

LoadingScreen::LoadingScreen(float w, float h)
{
	localH = h;
	localW = w;
	textConverter = new TextConverter();

	font.LoadDefault();
	font.Load("arial.ttf");
	text[0] = 
		"Blinky is a droid with an Ai companion created by Yer.\n"
		"Blinky comes with vast knowledge and is actually intelligent\n"
		"enough to create several ideas with provided information.\n"
		"In the story, Blinky must stay next to the portal until the player\n" 
		"finds an alternative power source as Blinky’s power source has been\n" 
		"damaged.Blinky acts as a shop keeper of kind where if you bring him\n" 
		"resources, he will tell you what he needs to rebuild the portal.\n"
		"You can arm and upgrade him with weapons to have self defence so\n"
		"that you can leave the portal and explore for more resources.\n"
		"Eventually, you can have him moving freely after finding a power\n"
		"source at which point he follows you around or stays guard in any\n" 
		"placement you seem fit, bringing new avenues for tactical combat.\n";


	text[1] = "Alumni \n are the titles given  \nto the graduates  \nof The Academy.  ";
	text[2] = "Yor  \nis the husband of Myne. \n Crafted many things Myne  \nuses on her daily adventures";


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
	textConverter->splitTextToLines(text[randomTextSelectionNum], localW - 790, localH / 100 * 70, 20);
 

	//for (auto lineOfText : allLines)
	//{
	//	font.DrawText(localW - 590, localH / 100 * 70 - lineHeight, lineOfText, CColor::White(), 15);
	//	lineHeight += 19;
	//}

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
