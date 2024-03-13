#include "headers/MyGame.h"
#include "headers/Cutscene.h"
#include "headers/UIDialogBox.h"

Cutscene::Cutscene(float w, float h)
{
	shipModel.LoadModel("test/test.obj");
	shipModel.SetScale(1150.f);

	//Cutscene Edges
	screenEdges.LoadImage("screenEdges.png");
	screenEdges.SetSize(w, h);
	screenEdges.SetPosition(w / 2, h / 2);

	//transition
	darkTransition.LoadImage("darkTransition.png");
	darkTransition.SetSize(w, h);
	darkTransition.SetPosition(w / 2, h / 2);
}

void Cutscene::init(float w, float h)
{
	//ship position Resets
	shipModel.SetPosition(1400, 3500, 1400);
	shipModel.SetSpeed(-75);

	//resets
	dialogSwitcherTimer = 0;
	dialogNumber = 0;
	blackScreenTimer = 0;
	shiprotationalAngelY = 0;
	//isCutscenePlaying =
}

void Cutscene::Update(Uint32 t, UIDialogBox& dialogBox)
{
	localDialogBox = &dialogBox;
	
	localTime = t;
	cutSceneOne();
	shipModel.Update(t);
}

void Cutscene::Draw2d(CGraphics* g)
{
	screenEdges.Draw(g);

	if (dialogNumber == 11)
	{
		darkTransition.Draw(g);
	}
}

void Cutscene::Draw3d(CGraphics* g)
{
	shipModel.Draw(g);

}

void Cutscene::cutSceneOne()
{
	if (dialogSwitcherTimer == 0 )
	{
		dialogSwitcherTimer = localTime + 8000;
		engineSound.Play("engineSound.wav", -1);
	}

 
	shipModel.Rotate(0, shiprotationalAngelY, 0);

	if (dialogSwitcherTimer < localTime && dialogNumber == 0)
	{
		dialogSwitcherTimer = localTime + 8000;
		localDialogBox->showBox(0, dialogNumber, 0);
		dialogNumber++;
	}



	if (dialogSwitcherTimer < localTime && dialogNumber == 1)
	{
		shiprotationalAngelY = 0.2;
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, 0);
		dialogNumber++;
	}


	if (dialogSwitcherTimer < localTime && dialogNumber == 2)
	{

		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, 0);
		dialogNumber++;
		shiprotationalAngelY = 0;
	}


	if (dialogSwitcherTimer < localTime && dialogNumber == 3)
	{
		
		dialogSwitcherTimer = localTime + 4000;	 
		shipModel.Rotate(0, 0, 50);
		shipModel.SetSpeed(-150);
		shiprotationalAngelY = 0.5;
		explosionSound.Play("explosion.wav");
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 4)
	{
		
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, 0);
		dialogNumber++;

	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 5)
	{
		
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 6)
	{
	
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 7)
	{
	
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 8)
	{
		
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 9)
	{

		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, 0);
		dialogNumber++;
	}


	if (dialogNumber == 10)
	{
		if(localDialogBox->isBoxShowen) localDialogBox->hideBox();
		dialogNumber++;
	}


	if (dialogNumber == 11 && isCutscenePlaying)
	{
		blackScreenTimer += 1;
		darkTransition.SetAlpha(blackScreenTimer);
	}

	if (blackScreenTimer >= 100)
	{
		isCutscenePlaying = false;
		engineSound.Pause();

	}

	shipModel.SetDirectionV(shipModel.GetRotationV());

}


