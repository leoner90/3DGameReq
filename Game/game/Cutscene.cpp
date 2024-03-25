#include "headers/MyGame.h"
#include "headers/Cutscene.h"
#include "headers/UIDialogBox.h"
#include "headers/Player.h"

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
	portal.LoadModel("portal/portalAnimated.md3");
	portal.SetScale(230.f);
	portal.SetPosition(400, 60, 300);
	portal.AddAnimation("portalOpen", 1, 39);

	portalVfx.LoadModel("rainDrop/rainDrop.obj");
	//rainDrop.SetY(50);
	portalVfx.SetScale(0.3f);
}

void Cutscene::init(float w, float h, Player& player)
{
	//ship position Resets
	shipModel.SetPosition(1400, 3500, 1400);
	shipModel.SetDirectionAndRotationToPoint(0, 0);
	shipModel.SetDirection(0, 0, 0);
	shipModel.SetRotation(0,0,0);
	shipModel.SetSpeed(-75);

	//resets
	dialogSwitcherTimer = 0;
	dialogNumber = 0;
	blackScreenTimer = 0;
	shiprotationalAngelY = 0;
	curentCutSceneNum = -1;
	cutSceneEndDimOn = false;
	delay = 0;

	//cutscene2
	localPlayer = &player;
	portal.PlayAnimation(0, 0, 0, false);
	localPlayer->playerModel.SetSpeed(0);
	cutsceneTwoStarted = false;
	CutSceneTwoReachedPortal = false;
	particleList.delete_all();

	//camera
	cutcceneCameraPosition = CVector(0, 0, 0);
}

void Cutscene::Update(Uint32 t, UIDialogBox& dialogBox)
{
	localDialogBox = &dialogBox;
	
	localTime = t;
	if (curentCutSceneNum == 0)
	{
		shipModel.Update(t);
		cutSceneOne();
		
	}
	
	if (curentCutSceneNum == 1)
	{
		localPlayer->playerModel.Update(localTime);
		particleList.Update(localTime);
		particleList.delete_if(true);
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
		if(blackScreenTimer < 40 )localPlayer->playerModel.Draw(g);
		particleList.Draw(g);
	}

}



void Cutscene::cutSceneOne()
{
	cutcceneCameraPosition = CVector(shipModel.GetX(), shipModel.GetY() + 140, shipModel.GetZ() + 140);
	if (dialogSwitcherTimer == 0 )
	{
		dialogSwitcherTimer = localTime + 8000;
		engineSound.Play("engine.wav", -1);
		engineSound.SetVolume(90);
	}

 
	shipModel.Rotate(0, shiprotationalAngelY, 0);

	if (dialogSwitcherTimer < localTime && dialogNumber == 0)
	{
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
		explosionSound.SetVolume(30);
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
		engineSound.Pause();
		crashSound.Play("crash.wav", -1);
		crashSound.SetVolume(60);

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
		engineSound.Pause();
		isCutscenePlaying = false;
		crashSound.Pause();
	}

	shipModel.SetDirectionV(shipModel.GetRotationV());
}


void Cutscene::cutSceneTwo()
{
	if (!cutsceneTwoStarted)
	{
		localPlayer->playerModel.SetPosition(1500, 0, 1500);
		shiprotationalAngelY = 0.3;
		cutsceneTwoStarted = true;
		localDialogBox->showBox(0, 22, 22, 3, 3800);
		
		localPlayer->playerModel.SetSpeed(300);
		localPlayer->playerModel.PlayAnimation("runF", 22, true);
		localPlayer->playerModel.SetDirectionAndRotationToPoint(400, 300);
	}

	CVector displ =CVector(400, 0, 300) - localPlayer->playerModel.GetPositionV() ;
	float distance = hypot(displ.GetX(), displ.GetZ());

 

	if (distance <= 50 && !CutSceneTwoReachedPortal)
	{
		localPlayer->playerModel.SetSpeed(0);
		localPlayer->playerModel.PlayAnimation("idle", 22, true);
		portal.PlayAnimation("portalOpen", 8, false);
		CutSceneTwoReachedPortal = true;
		delay = 3500 + localTime;

		for (int i = 0; i < 750; i++)
		{
			//CModel* p = new CLine(localPlayer->playerModel.GetX(), localPlayer->playerModel.GetY(), localPlayer->playerModel.GetZ(), 30, CColor::Blue());

			CModel* p = portalVfx.Clone();
			p->SetPositionV(localPlayer->playerModel.GetPositionV());
			p->SetRotation(float(rand() % 180), float(rand() % 360), 0);
			p->SetDirectionV(p->GetRotationV()); // align direction with rotation
			p->SetSpeed(rand() % 250);
			p->Die(3800);
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

		cutsceneTwoStarted = false;
		CutSceneTwoReachedPortal = false;

		particleList.delete_all();
	}

	cutcceneCameraPosition = CVector(localPlayer->playerModel.GetX() - 300, localPlayer->playerModel.GetY() + 600 , localPlayer->playerModel.GetZ());
	portal.Update(localTime);
}



void Cutscene::startCutscene(int sceneNum)
{
	curentCutSceneNum = sceneNum;
	isCutscenePlaying = true;
	cutSceneEndDimOn = false;
	blackScreenTimer = 0;
}


