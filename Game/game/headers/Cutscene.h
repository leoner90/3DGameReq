#pragma once
class Player;
class UIDialogBox;

class Cutscene
{
public:
	Cutscene(float w, float h);
	
	//Main Functions
	void init(float w, float h, Player& player);
	void Update(Uint32 t, UIDialogBox& dialogBox);
	void Draw2d(CGraphics* g);
	void Draw3d(CGraphics* g);

	//Cutscenes
	void cutSceneOne();
	void cutSceneTwo();
	void startCutscene(int sceneNum);


	//TODO GETTERS SETTERS
	bool isCutscenePlaying = true;
	int dialogNumber;
	float shiprotationalAngelY;
	int curentCutSceneNum;
	float dialogSwitcherTimer;
	CVector cutcceneCameraPosition;

private:
	enum Cutsenes { START, END };

	CModel shipModel;
	CSoundPlayer explosionSound, engineSound, crashSound;
	CSprite screenEdges, darkTransition;
	UIDialogBox* localDialogBox;
	CSoundPlayer test;
	CModel portalPartOne, portalPartTwo, portalPartThree, portalPartFour;
	CModelList particleList;
	CVector initRotation, initPos, initVelocity;
	Player* localPlayer;
	CModel portalVfx;
	CModelMd3 portal;

	float localTime;
	bool cutSceneEndDimOn;
	float delay;
	bool cutsceneTwoStarted, CutSceneTwoReachedPortal; // to redo
	int blackScreenTimer;
};

 