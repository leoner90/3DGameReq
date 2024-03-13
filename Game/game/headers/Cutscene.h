#pragma once

class UIDialogBox;

class Cutscene
{
public:
	Cutscene(float w, float h);
 
	enum Cutsenes {START,END};
	CModel shipModel;
	void init(float w, float h);
	void Update(Uint32 t, UIDialogBox& dialogBox);
	void Draw2d(CGraphics* g);
	void Draw3d(CGraphics* g);
	void cutSceneOne();
	
	float dialogSwitcherTimer;
	int dialogNumber;
	bool isCutscenePlaying = true;
	float shiprotationalAngelY;
private:
	int blackScreenTimer;

	CSoundPlayer explosionSound, engineSound;

	CSprite screenEdges, darkTransition;

	float localTime;

	UIDialogBox* localDialogBox;
};

 