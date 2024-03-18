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

	//Cutscene 2
	//model
	playerModel.LoadModel("Player/myra.md3");
	playerModel.LoadTexture("Player/myraTextures.png");
	playerModel.SetScale(25.5f);
	playerModel.SetPosition(1500, 0, 1500);

	//animation
	playerModel.AddAnimation("runF", 255, 276);
	playerModel.AddAnimation("idle", 1, 249);


	portal.LoadModel("portal/portalAnimated.md3");
	portal.SetScale(230.f);
	portal.SetPosition(400, 80, 300);
	portal.AddAnimation("portalOpen", 1, 45);

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
	curentCutSceneNum = 0;
	cutSceneEndDimOn = false;
	delay = 0;


	cutsceneTwoStarted = false;
	CutSceneTwoReachedPortal = false;
	
}

void Cutscene::Update(Uint32 t, UIDialogBox& dialogBox)
{
	localDialogBox = &dialogBox;
	
	localTime = t;
	if (curentCutSceneNum == 0)
	{
		cutSceneOne();
		shipModel.Update(t);
	}
	
	if (curentCutSceneNum == 1)
	{
		cutSceneTwo();
	}
}

void Cutscene::Draw2d(CGraphics* g)
{
	screenEdges.Draw(g);

	if (cutSceneEndDimOn)
	{
		darkTransition.Draw(g);
	}
}

void Cutscene::Draw3d(CGraphics* g)
{
	if (curentCutSceneNum == 0) shipModel.Draw(g);
	if (curentCutSceneNum == 1)
	{
		portal.Draw(g);
		portalPartOne.Draw(g);
		portalPartTwo.Draw(g);
		portalPartThree.Draw(g);
		portalPartFour.Draw(g);
		playerModel.Draw(g);
		particleList.Draw(g);
	}

}

void Cutscene::cutSceneTwo()
{
	if (!cutsceneTwoStarted)
	{
		cutsceneTwoStarted = true;
		localDialogBox->showBox(0, 22, 22, 4000);

		playerModel.SetSpeed(300);
		playerModel.PlayAnimation("runF", 22, true);
		playerModel.SetDirectionAndRotationToPoint(400, 300);
	}



	CVector displ = playerModel.GetPositionV() - CVector(400,0,300);
	float distance = hypot(displ.GetX(), displ.GetZ());


	if (distance <= 50 && !CutSceneTwoReachedPortal)
	{
		playerModel.SetSpeed(0);
		playerModel.PlayAnimation("idle", 22, true);
		portal.PlayAnimation("portalOpen", 9, false);
		CutSceneTwoReachedPortal = true;
		delay = 3500 + localTime;

		for (int i = 0; i < 1500; i++)
		{
			CModel* p = new CLine(playerModel.GetX(), playerModel.GetY(), playerModel.GetZ(), 30, CColor::Blue());
			p->SetRotation(float(rand() % 180), float(rand() % 360), 0);
			p->SetDirectionV(p->GetRotationV()); // align direction with rotation
			p->SetSpeed(rand() % 250);
			p->Die(4000);
			particleList.push_back(p);
		}
	} 

 

	if (CutSceneTwoReachedPortal && delay < localTime)
	{
		if (!cutSceneEndDimOn) cutSceneEndDimOn = true;
		blackScreenTimer += 1;
		darkTransition.SetAlpha(blackScreenTimer);
	}

	if (blackScreenTimer >= 100)
	{
		isCutscenePlaying = false;
		engineSound.Pause();

	}

 
	
	cutcceneCameraPosition = CVector(playerModel.GetX(), playerModel.GetY(), playerModel.GetZ());
	portal.Update(localTime);
	playerModel.Update(localTime);
	particleList.Update(localTime);
}

void Cutscene::cutSceneOne()
{
	cutcceneCameraPosition = CVector(shipModel.GetX(), shipModel.GetY(), shipModel.GetZ());
	if (dialogSwitcherTimer == 0 )
	{
		dialogSwitcherTimer = localTime + 8000;
		engineSound.Play("engineSound.wav", -1);
	}

 
	shipModel.Rotate(0, shiprotationalAngelY, 0);

	if (dialogSwitcherTimer < localTime && dialogNumber == 0)
	{
		test.Play("1.wav");
		dialogSwitcherTimer = localTime + 8000;
		localDialogBox->showBox(0, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}



	if (dialogSwitcherTimer < localTime && dialogNumber == 1)
	{
		shiprotationalAngelY = 0.2;
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}


	if (dialogSwitcherTimer < localTime && dialogNumber == 2)
	{

		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, dialogNumber, 0);
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
		localDialogBox->showBox(0, dialogNumber, dialogNumber, 0);
		dialogNumber++;

	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 5)
	{
		
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 6)
	{
	
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 7)
	{
	
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(0, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 8)
	{
		
		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}

	if (dialogSwitcherTimer < localTime && dialogNumber == 9)
	{

		dialogSwitcherTimer = localTime + 4000;
		localDialogBox->showBox(1, dialogNumber, dialogNumber, 0);
		dialogNumber++;
	}


	if (dialogNumber == 10)
	{
		if(localDialogBox->isBoxShowen) localDialogBox->hideBox();
		dialogNumber++;
	}


	if (dialogNumber == 11 && isCutscenePlaying)
	{
		if (!cutSceneEndDimOn) cutSceneEndDimOn = true;
		blackScreenTimer += 2;
		darkTransition.SetAlpha(blackScreenTimer);
	}

	if (blackScreenTimer >= 100)
	{
		isCutscenePlaying = false;
		engineSound.Pause();

	}

	shipModel.SetDirectionV(shipModel.GetRotationV());

}



void Cutscene::startCutscene(int sceneNum)
{
	curentCutSceneNum = sceneNum;
	isCutscenePlaying = true;
	cutSceneEndDimOn = false;
	blackScreenTimer = 0;
}


