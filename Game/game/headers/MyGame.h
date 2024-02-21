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
	CSprite startScreen, mainMenushowControlers;
	CFont font;

	// Main Objects
	Map* map;
	Player* player;
	PlayerInterface* playerInterface;
	//Enemy* enemy;

	// Game Funtions
	virtual void OnInitialize();
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);
	void CameraControl(CGraphics* g);

	void MainMenyController(SDLKey sym);

	//INIT SPRITES AND MODELS
	void InitSpritesAndModels();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void  OnRButtonDown(Uint16 x, Uint16 y);

	//Game Modes
	enum GameModes{MAIN_MENU, CHAR_STATS, SHOP, IN_GAME, CUTSCENE, SHOW_CONTROLLERS};
	enum mainMenuOptions{NEW_GAME, CONTROLS, EXIT};
	bool gameStarted;
	int mainMenuOptionSelection;
	void MaiMenuDraw(CGraphics* g);
	int currentMenuState;
	//sound
	CSoundPlayer mainBgMusic;


	void  OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle);
	CVector currentMousePos;
};