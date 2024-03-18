#include "Game.h"
#include "headers/UIDialogBox.h"
#include "headers/textConverter.h"
 


//UIDialogBox::UIDialogBox() {  }

void UIDialogBox::init(float w, float h)
{
	localW = w;
	localH = h;
	font.LoadDefault();
	textConverter = new TextConverter();

	 
 

	dialogBoxBg.LoadImage("dialogBox3.png");
	dialogBoxBg.SetSize(w , 180);
	dialogBoxBg.SetPosition(w / 2  , 110);

	initPosSpeakerImages;
	speakerImg.LoadImage("robotLogo.png");
	speakerImg.SetSize(120, 100);
	speakerImg.SetPosition( 210, 100);


	MyneImg.LoadImage("MyraLogo.png");
	MyneImg.SetSize(120, 100);
	MyneImg.SetPosition(190, 100);

 
	isBoxShowen = false;

	name[MYNE] = "Myne";
	name[BLINKY] = "Blinky";

	text[0] = "Looks Like this planet has a lot of resources , \n we have to check this out";
	text[1] = "I detect many organic life forms on the surface,\n but their intelligence leaves much to be desired.";
	text[2] = "We have to be careful down there";
	text[3] = "expl";
	text[4] = "What Was that?????";
	text[5] = "Sensor detects a malfunction in engine section";
	text[6] = "Malfunction?? That sounds more like a big explodition";
	text[7] = "Ship Control is down , we are falling";
	text[8] = "It was a pleasure to work with you. gg";

	text[9] = "BE AWARE Enemies Are Coming!!!!!!!!!!!!!!!!";
	text[10] = "PORTAL IS UNDER ATTACK";
	text[11] = "PORTAL OVERLOADED - IT TAKES SOME TIME TO START AGAIN ";
	text[12] = "PRESS E FOR SHOPING";
	text[13] = "BE AWARE Enemies Are Coming!!!!!!!!!!!!!!!!";



	text[15] =
		"What a landing...\n"
		"It’s a miracle that I survived.\n";
	text[16] = "Looks like this ship will never fly again.\n";
	text[17] = 
		"I wonder how Blinky is doing, although he’s a robot\n"
		 "he’s not immortal, it seems that during landing he was thrown towards that strange portal";

	text[18] = "Myne!!! You alive.\n";
	text[19] = "Wonderful, I started to charge this ancient portal, it's our ticket home\n";
	text[20] = 
			"But I'm afraid it's woke up all the monsters around, you have to survive \n"
			"while the portal is charging";

	text[21] = "Portal is back to life";

	text[22] = "Portal is charged , faster it's time to go";
		 


	autoHideTimer = 0;
	hideInSec = 0;
}

void UIDialogBox::OnUpdate(long t, bool fullWidth)
{
	localTime = t;
 
	/*
	//full width in cutsenes , 
	if (fullWidth)
	{
		dialogBoxBg.SetSize(localW, dialogBoxBg.GetY());
		dialogBoxBg.SetPosition(localW / 2, dialogBoxBg.GetY());
	}
	else 
	{
		dialogBoxBg.SetSize(localW - 300, dialogBoxBg.GetY()); // 300 size of RADAR
		dialogBoxBg.SetPosition(localW / 2 - 150, dialogBoxBg.GetY());
	}

	*/

	//hide
	if ((autoHideTimer && isBoxShowen) || (hideInSec < localTime && hideInSec != 0 ))
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

	//show 
	if (!autoHideTimer && isBoxShowen && dialogBoxShowSpeed <= 20)
	{
		dialogBoxShowSpeed += 5;
		dialogBoxBg.SetPosition(dialogBoxBg.GetX(), dialogBoxBg.GetY() + dialogBoxShowSpeed);
		speakerImg.SetPosition(speakerImg.GetX(), speakerImg.GetY() + dialogBoxShowSpeed);
		MyneImg.SetPosition(speakerImg.GetX(), speakerImg.GetY() + dialogBoxShowSpeed);
	} 
 
	 

	dialogBoxBg.Update(t);
	speakerImg.Update(t);
	MyneImg.Update(t);

}

void UIDialogBox::OnDraw(CGraphics* g)
{
	if (isBoxShowen )
	{
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

	if (textShow && dialogBoxShowSpeed >= 20)
	{
		font.DrawText(localW * 0.25, 110, name[speaker], CColor::Yellow(), 26);
		textConverter->splitTextToLines(text[dialogNumber], localW * 0.25, 80, 22);	
	}
 	
}

void UIDialogBox::showBox(int speakerId, int textIdStart, int textIdEnd, int priority,  float autohideBoxin)
{
	if (autohideBoxin != -1) hideInSec = localTime + autohideBoxin;
	if (currentPriority > priority) return;
	//set back to origin pos
	//speakerImg.SetPosition(125, 0);
	//dialogBoxBg.SetPosition(localW / 2  , 0);



	dialogBoxBg.SetPosition(localW / 2, 0);
	speakerImg.SetPosition(210, 0);
	MyneImg.SetPosition(210, 0);




	isBoxShowen = true;
	autoHideTimer = false;
	localautohideTime = autohideBoxin;
	
	speaker = speakerId;
	dialogNumber = textIdStart;
	dialogEndNumber = textIdEnd;
	//dialogBoxHideSpeed = 0;
	currentPriority = priority;
	dialogBoxShowSpeed = 0;
	textShow = true;
}

void UIDialogBox::hideBox()
{
	//isBoxShowen = false;
	//dialogBoxHideSpeed = 0;
	textShow = false;
	autoHideTimer = true;
	currentPriority = -1;

}




 

 