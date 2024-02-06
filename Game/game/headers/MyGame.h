#pragma once
#include "Game.h"

class CMyGame : public CGame
{
private:
    // Declare Sprites
    CSprite startScreen , map2dExample;
	CFloor floor;
	CFont font;
	CHealthBar hbar;

	// Game Funtions
	virtual void OnInitialize();
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);
	void CameraControl(CGraphics* g);

	//INIT SPRITES AND MODELS
	void InitSpritesAndModels();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

	//bool for 2d or 3d Game
	bool gameOption;
};