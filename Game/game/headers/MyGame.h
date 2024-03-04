#pragma once
#include "Game.h"

//Forwward class declaration
class Map;
class Player;
class PlayerInterface;
class Enemy;
class Shop;

class CMyGame : public CGame
{
private:
	CSprite startScreen, mainMenushowControlers, CharStatsMenu;
	CFont font;
 
	// Main Objects
	Map* map;
	Player* player;
	PlayerInterface* playerInterface;
	Shop* shop;
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

	//Mouse Control
	void OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle);
	void OnWheelUp(Uint16 x, Uint16 y);
	void OnWheelDown(Uint16 x, Uint16 y);

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void  OnLButtonDown(Uint16 x, Uint16 y);

	//Game Modes
	enum GameModes{MAIN_MENU, CHAR_STATS, SHOP, IN_GAME, CUTSCENE, SHOW_CONTROLLERS};
	enum mainMenuOptions{NEW_GAME, CONTROLS, EXIT};

	GameModes currentMenuState;
	bool gameStarted;
	int mainMenuOptionSelection;

	//fog
	void EnableFog();

	void MaiMenuDraw(CGraphics* g);
	void CharStatsDraw(CGraphics* g);
	//sound
	CSoundPlayer mainBgMusic;

	void OnMButtonDown(Uint16 x, Uint16 y);
	void OnMButtonUp(Uint16 x, Uint16 y);

	bool cameraMovement;

	CVector currentMousePos;

	CVector cameraControlMouseInitPose;

	// the skydome is a CModel too
	CModel skydome;
};