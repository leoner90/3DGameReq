#pragma once
#include "Game.h"

//Forwward class declaration
class Map;
class Player;
class PlayerInterface;
class Enemy;

class CMyGame : public CGame
{
private:
	CSprite startScreen;
	CFont font;

	// Main Objects
	Map* map;
	Player* player;
	PlayerInterface* playerInterface;
	Enemy* enemy;

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
	void  OnRButtonDown(Uint16 x, Uint16 y);

	//sound
	CSoundPlayer mainBgMusic;
};