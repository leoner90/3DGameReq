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

 
	isBoxShowen = true;
}

void UIDialogBox::OnUpdate(long t)
{

	if (isBoxShowen)
	{
		float pos = dialogBoxBg.GetY() - 10;
		dialogBoxBg.SetPosition(dialogBoxBg.GetX(), pos);
		if (pos < -dialogBoxBg.GetHeight() / 2) isBoxShowen = false;
	}

}

void UIDialogBox::OnDraw(CGraphics* g)
{
	if (isBoxShowen)
	{
		dialogBoxBg.Draw(g);
		speakerImg.Draw(g);

		font.DrawText(localW * 0.2, 120, "Blinky", CColor::Yellow(), 28);
		font.DrawText(localW * 0.2, 100, "Ship is critycly damaged, the portal can help as survive, " , CColor::White(), 22);
		font.DrawText(localW * 0.2, 80, "I'm starting to activate the portal ", CColor::White(), 22);
		font.DrawText(localW * 0.2, 60, "unfortunatly the energy blast will attract all the creatures near by", CColor::White(), 22);
	 

	}
	
}

void UIDialogBox::showBox()
{
	isBoxShowen = true;
}

void UIDialogBox::hideBox()
{
	isBoxShowen = false;
}



