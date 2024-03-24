#pragma once
class Player;
class UIDialogBox;

class Cutscene
{
public:
	Cutscene(float w, float h);
 
	enum Cutsenes {START,END};
	CModel shipModel;
	void init(float w, float h, Player& player);
	void Update(Uint32 t, UIDialogBox& dialogBox);
	void Draw2d(CGraphics* g);
	void Draw3d(CGraphics* g);
	void cutSceneOne();
	void cutSceneTwo();
	
	float dialogSwitcherTimer;
	int dialogNumber;
	bool isCutscenePlaying = true;
	float shiprotationalAngelY;
	int curentCutSceneNum;
	void startCutscene(int sceneNum);

	CVector cutcceneCameraPosition;

private:
	int blackScreenTimer;

	CSoundPlayer explosionSound, engineSound;

	CSprite screenEdges, darkTransition;

	float localTime;

	UIDialogBox* localDialogBox;

	CSoundPlayer test;
	bool cutSceneEndDimOn;

	CModelMd3 portal;

	float delay;
	bool cutsceneTwoStarted, CutSceneTwoReachedPortal; // to redo
	CModel portalPartOne, portalPartTwo, portalPartThree, portalPartFour;

	CModelList particleList;

	CVector initRotation, initPos, initVelocity;

	Player* localPlayer;

	CModel portalVfx;
};

 