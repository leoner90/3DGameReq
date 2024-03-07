#include "Game.h"
#include "headers/UIDialogBox.h"
#include "string"
void UIDialogBox::init(float w, float h)
{
	localW = w;
	localH = h;
	font.LoadDefault();

	dialogBoxBg.LoadImage("dialogBox.png");
	dialogBoxBg.SetSize(w, 160);
	dialogBoxBg.SetPosition(w / 2, 80);


	speakerImg.LoadImage("robotLogo.png");
	speakerImg.SetSize(150, 120);
	speakerImg.SetPosition( 125, 80);


	MyneImg.LoadImage("MyraLogo.png");
	MyneImg.SetSize(150, 120);
	MyneImg.SetPosition(125, 80);

 
	isBoxShowen = false;

	name[MYNE] = "Myne";
	name[BLINKY] = "Blinky";
	text[0] = "PRESS E FOR SHOPING";
	text[1] = "BE AWARE Enemies Are Coming!!!!!!!!!!!!!!!!";

	autoHideTimer = 0;
	hideInSec = 0;
}

void UIDialogBox::OnUpdate(long t)
{
	localTime = t;
 
	//hide
	if ((autoHideTimer && isBoxShowen) || (hideInSec < localTime && hideInSec != 0 ))
	{
		dialogBoxHideSpeed += 8;
		dialogBoxBg.SetPosition(dialogBoxBg.GetX(), dialogBoxBg.GetY() - dialogBoxHideSpeed);
		speakerImg.SetPosition(speakerImg.GetX(), speakerImg.GetY() - dialogBoxHideSpeed);

		if (dialogBoxHideSpeed >= dialogBoxBg.GetHeight() / 4)
		{
			dialogBoxHideSpeed = 0;
			autoHideTimer = false;
			isBoxShowen = false;
			textShow = false;
			speakerImg.SetPosition(125, 0);
			dialogBoxBg.SetPosition(localW / 2, 0);
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
	} 
 
	 

	//dialogBoxBg.Update(t);
	//speakerImg.Update(t);

}

void UIDialogBox::OnDraw(CGraphics* g)
{
	if (isBoxShowen )
	{
		dialogBoxBg.Draw(g);
		switch (speaker)
		{
			case 0: 
				speakerImg.Draw(g);
				break;
			case 1: 
				MyneImg.Draw(g);
				break;

		}
		

	}

	if (textShow && dialogBoxShowSpeed >= 20)
	{
		font.DrawText(localW * 0.2, 110, name[speaker], CColor::Yellow(), 28);
		font.DrawText(localW * 0.2, 80, text[dialogNumber], CColor::White(), 22);
	}
 	
}

void UIDialogBox::showBox(int speakerId, int textId, int priority, float autohideBoxin)
{
	if (autohideBoxin != -1) hideInSec = localTime + autohideBoxin;
	if (currentPriority > priority) return;
	//set back to origin pos
	speakerImg.SetPosition(125, 0);
	dialogBoxBg.SetPosition(localW / 2, 0);
	isBoxShowen = true;
	autoHideTimer = false;
	
	speaker = speakerId;
	dialogNumber = textId;
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



