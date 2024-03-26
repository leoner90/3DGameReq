#include "Game.h"
#include "headers/UIDialogBox.h"
#include "headers/textConverter.h"
 


UIDialogBox::UIDialogBox(float w, float h)
{ 
	localW = w;
	localH = h;

	font.LoadDefault();
	dialogBoxBg.LoadImage("dialogBox3.png");
	dialogBoxBg.SetSize(w, 180);
 
	speakerImg.LoadImage("robotLogo.png");
	speakerImg.SetSize(120, 100);
 
	MyneImg.LoadImage("MyraLogo.png");
	MyneImg.SetSize(120, 100);
 

	dialogBoxShowSpeed = 20;
	speakerImgMarginTop = 80;


	//DIALOGS
	name[MYNE] = "Myne";
	name[BLINKY] = "Blinky";
	//cutscene 1
	text[0] = "How much longer, Blinky? This is taking forever. ";
	text[1] = "Just a few more moments, Myne.\nI am almost done scanning the planet.\nJust need to get some reading on the portal system.";
	text[2] = "God, we are so far away from home. It’s going to take forever to get back. Honestly, why would they bother with-";
	text[3] = "expl";
	text[4] = "What Was that?????";
	text[5] = "Myne, somethings- hap- can’t- repon- shutting- dow-";
	text[6] = "Blinky? Hey! Blinky!";
	text[7] = "The ship control is non-responsive, what the hell? What is going on? ";
	text[8] = "Myne- They’re- Not- Help-";

	//other
	text[9] = "Enemies incoming! ";
	text[10] = "PORTAL IS UNDER ATTACK";
	text[11] = "PORTAL OVERLOADED - IT WILL TAKE SOME TIME TO START AGAIN ";
	text[12] = "PRESS E to Shop";
	text[13] = "Enemies incoming! ";

	//start of the game
	text[15] =
		"Well at least the eject system worked.\n"
		"Blinky is out. What in the world is going on? ";

	text[16] = "The portal. I need to find that portal and \n hope it’s still operational. It’s the only way to get back. ";
	text[17] = " Hold tight, Blinky. I’ll get you working again and get us out of here.";
 


	text[18] = "Myne! What happened? Where are we?\n  Wait are we on land? \n Is that the portal?";
	text[19] = "Yes. We crashed right after you shut down.\n The ship is totalled. Listen? Can you start the portal? ";
	text[20] =
		"Yes. I can. I am wired in to the system. Give me a moment and I- \n"
		"Myne, the system- it senses tremors. Myne, we have incoming";


	text[21] = "Portal is back to life";
	text[22] = "Portal is fully charged. Let’s get out of here, Myne.";

}

void UIDialogBox::init()
{
	textConverter = new TextConverter();
	autoHideTimer = false;
	isBoxShowen = false;
	textShow = false;
	dialogBoxHideSpeed = 0;
	currentPriority = -1;
	autoHideTimer = 0;
	hideInSec = 0;
	speakerImgMarginLeft = 200;
	textMarginLeft = 0.25;
}

void UIDialogBox::OnUpdate(long t, bool fullWidth)
{
	localTime = t;
	
	//full width in cutsenes , 
	if (fullWidth )
	{
		speakerImgMarginLeft = 200;
		dialogBoxBg.SetSize(localW, dialogBoxBg.GetHeight());
		dialogBoxBg.SetPosition(localW / 2, dialogBoxBg.GetY());
	}
	else 
	{
		dialogBoxBg.SetSize(localW - 520, dialogBoxBg.GetHeight());  
		dialogBoxBg.SetPosition(localW / 2 + 80 , dialogBoxBg.GetY());
		speakerImgMarginLeft = 500;
		textMarginLeft = 0.4;
	}
 

	//hide
	if ((autoHideTimer && isBoxShowen) || (hideInSec < localTime && hideInSec != 0 )) // manual hide called or autohide(hideInSec) timer is off
	{
		//if dialog is not one , reset timer and change text id to next one
		if (dialogNumber < dialogEndNumber)
		{
			dialogNumber++; // next text
			hideInSec = t + localautohideTime;
			return;
		}


		dialogBoxHideSpeed += 8;
		dialogBoxBg.SetPosition(dialogBoxBg.GetX(), dialogBoxBg.GetY() - dialogBoxHideSpeed);
		speakerImg.SetPosition(speakerImg.GetX(), speakerImg.GetY() - dialogBoxHideSpeed);
		MyneImg.SetPosition(speakerImg.GetX(), speakerImg.GetY() - dialogBoxHideSpeed);

		if (dialogBoxHideSpeed >= dialogBoxBg.GetHeight() / 4)
		{
			dialogBoxHideSpeed = 0;
			autoHideTimer = false;
			isBoxShowen = false;
			textShow = false;
			hideInSec = 0;
			currentPriority = -1;
		}
	} 

	//show dialogBox 
	if (!autoHideTimer && isBoxShowen && dialogBoxBg.GetY() < dialogBoxBg.GetHeight() / 2)
	{
		dialogBoxBg.SetY(dialogBoxBg.GetY() + dialogBoxShowSpeed);

		//avatar centration
		if (speakerImg.GetY() <= speakerImgMarginTop)
		{
			speakerImg.SetY(speakerImg.GetY() + dialogBoxShowSpeed);
			MyneImg.SetY(MyneImg.GetY() + dialogBoxShowSpeed);
		}
		
	} 
 
 

}

void UIDialogBox::OnDraw(CGraphics* g)
{
	if (isBoxShowen )
	{
		//cheating
		if (dialogNumber == 19) speaker = 0;
		if (dialogNumber == 20) speaker = 1;

		dialogBoxBg.Draw(g);
		switch (speaker)
		{
			case 0: 
				MyneImg.Draw(g);
				break;
			case 1: 
				speakerImg.Draw(g);
				break;
		}
	}

	if (textShow && dialogBoxBg.GetY() >= dialogBoxBg.GetHeight() / 2)
	{
		font.DrawText(localW * textMarginLeft, 110, name[speaker], CColor::Yellow(), 26);
		textConverter->splitTextToLines(text[dialogNumber], localW * textMarginLeft, 80, 22);
	}
 	
}

void UIDialogBox::showBox(int speakerId, int textIdStart, int textIdEnd, int priority,  float autohideBoxin)
{
	if (autohideBoxin != -1) hideInSec = localTime + autohideBoxin;
	if (currentPriority > priority) return;

	dialogBoxBg.SetPosition(localW / 2, -dialogBoxBg.GetHeight() / 2);
	speakerImg.SetPosition(speakerImgMarginLeft, -speakerImg.GetHeight() / 2);
	MyneImg.SetPosition(speakerImgMarginLeft, -MyneImg.GetHeight() / 2);


	isBoxShowen = true;
	autoHideTimer = false;
	textShow = true;
	localautohideTime = autohideBoxin;

	speaker = speakerId;
	dialogNumber = textIdStart;
	dialogEndNumber = textIdEnd;
	currentPriority = priority;
}

void UIDialogBox::hideBox()
{
	textShow = false;
	autoHideTimer = true;
	currentPriority = -1;
}




 

 