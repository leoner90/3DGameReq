#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/Enemy.h"
#include "headers/Map.h"


std::vector<Enemy*> AllEnemies; // to make proper enemies init

//*************** INIT ***************
void CMyGame::OnInitialize()
{
	currentMenuState = MAIN_MENU;
	mainMenuOptionSelection = NEW_GAME;
	gameStarted = false;

	// Main Objects
	delete map;
	map = new Map();

	delete player;
	player = new Player();

	delete playerInterface;
	playerInterface = new PlayerInterface();
 

	//music
	mainBgMusic.Stop();
	mainBgMusic.Play("mainBg.wav");
	mainBgMusic.SetVolume(35);

	//StartScreen Models
	InitSpritesAndModels();

	//Main inits
	player->init();
	map->init();
	
	playerInterface->init(Width,Height);

	Light.Enable();
	font.LoadDefault();

	//ENEMIES CREATION
	
	for (auto enemy : AllEnemies) {
		delete enemy;
	}

	AllEnemies.clear();

	for (int i = 0; i < 67; i++) 
	{
		AllEnemies.push_back(new Enemy());
	}

	int i = 400;
	int z = 300;
	bool enemyType = 0;
	for (auto enemy : AllEnemies) {
		enemy->init(i,100, z, enemyType);
		i += 100;
		z += 50 + rand() % 150;
		enemyType = !enemyType;
	}

 
}

//*************** UPDATE ***************
void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) return;
	map->OnUpdate(GetTime());
	player->OnUpdate(GetTime(), IsKeyDown(SDLK_d) , IsKeyDown(SDLK_a) , IsKeyDown(SDLK_w), IsKeyDown(SDLK_s), *map , AllEnemies);
	playerInterface->OnUpdate(map->portal.GetHealth());

	int i = 0;
	for (auto enemy : AllEnemies) {
		
		enemy->OnUpdate(GetTime(), *player);

		//* if regular enemie dead -> delete;
		if (enemy->enemyModel.GetHealth() <= 0) {
			AllEnemies.erase(AllEnemies.begin() + i);
			delete enemy;
		}
		
		i++;
	}

}

//*************** 2D RENDER ***************
void CMyGame::OnDraw(CGraphics* g)
{

	if (IsMenuMode())
	{
		
		MaiMenuDraw(g);
		return;
	}

	playerInterface->OnDraw(g);
	
}

//*************** 3D RENDER ***************
void CMyGame::OnRender3D(CGraphics* g)
{ 
	CameraControl(g);
	map->OnRender3D(g);
	player->OnRender3D(g, world);
	

	for (auto enemy : AllEnemies) {
		enemy->OnRender3D(g);
	}

	ShowBoundingBoxes();
	ShowCoordinateSystem();
}

//*************** CAMERA ***************
void CMyGame::CameraControl(CGraphics* g)
{
	world.rotation.x = 60;     // rotation of game world around x-axis
	world.rotation.y = -35;   
	world.position.z = 700;
	world.scale = 1.0f;	       
	 

	// ------ Global Transformation 
	glScalef(world.scale, world.scale, world.scale);  // scale the game world
	glTranslatef(world.position.x, world.position.y, world.position.z);  // translate game world
	glRotatef(world.rotation.x, 1, 0, 0);	// rotates game world around x-axis
	glRotatef(world.rotation.y, 0, 1, 0);

	// -----  Player camera control (translate game world to player position )
	glTranslatef(-player->playerModel.GetX() , 0, -player->playerModel.GetZ());

	UpdateView();
	Light.Apply();
}



void CMyGame::InitSpritesAndModels()
{
	// start screen
	startScreen.LoadImage("startScreen.jpg");
	startScreen.SetSize((float)Width, (float)Height);
	startScreen.SetPosition((float)Width / 2, (float)Height / 2);

	mainMenushowControlers.LoadImage("mainMenushowControlers.jpg");
	mainMenushowControlers.SetSize((float)Width, (float)Height);
	mainMenushowControlers.SetPosition((float)Width / 2, (float)Height / 2);
}

//*************** KEYBOARD EVENTS ***************



void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Player KeyBoard Controller
	player->OnKeyDown(sym , currentMousePos);

	//Start
	MainMenyController(sym);
}

void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{
	player->OnRButtonDown(ScreenToFloorCoordinate(x, y), currentMousePos, GetTime());
}

void CMyGame::MaiMenuDraw(CGraphics* g)
{
	if (currentMenuState == SHOW_CONTROLLERS) 
	{
		mainMenushowControlers.Draw(g);
		font.DrawText((float)Width / 2 - 220.f, 360, "RIGHT MOUSE BTN. - MOVE CHARACTER", CColor::White() , 32);
		font.DrawText((float)Width / 2 - 220.f, 290, "Q - SHOOT", CColor::White(), 32);
		font.DrawText((float)Width / 2 - 220.f, 220, "W - SKILL_1", CColor::White(), 32);
		font.DrawText((float)Width / 2 - 220.f, 150, "E - INTERACT", CColor::White(), 32);

		font.DrawText((float)Width / 2 - 120.f, 30, "BACK", CColor::White(), 42);
	}
	else 
	{
		startScreen.Draw(g);
		font.DrawText((float)Width / 2 - 80.f, (float)Height / 2 + 80, "NEW GAME", mainMenuOptionSelection == NEW_GAME ? CColor::White() : CColor::LightGray(), 42);
		font.DrawText((float)Width / 2 - 80.f, (float)Height / 2 , "LOAD GAME", CColor::Gray(), 42);
		font.DrawText((float)Width / 2 - 80.f, (float)Height / 2 - 80  , "CONTROLS", mainMenuOptionSelection == CONTROLS ? CColor::White() : CColor::LightGray(), 42);
		font.DrawText((float)Width / 2 - 40.f, (float)Height / 2 - 160, "EXIT", mainMenuOptionSelection == EXIT ? CColor::White() : CColor::LightGray(), 42);
	}
}


void CMyGame::MainMenyController(SDLKey sym)
{
	// Call / Cancel Menu on ESC btn. pressed
	if (!IsMenuMode() && sym == SDLK_ESCAPE && gameStarted )
	{
		SetGameMode(MODE_MENU);
		currentMenuState = MAIN_MENU;
	}
	else if (IsMenuMode() && sym == SDLK_ESCAPE && gameStarted && currentMenuState == MAIN_MENU)
	{
		SetGameMode(MODE_RUNNING);
		currentMenuState = IN_GAME;
	}

	////******* IF NOT IN MENU MODE -> RETURN
	if (!IsMenuMode()) return;

	//******* IF SHOW_CONTROLLERS
	if (currentMenuState == SHOW_CONTROLLERS )
	{
		if (sym == 13 || sym == SDLK_ESCAPE) currentMenuState = MAIN_MENU;
	}

	//******* IF MAIN_MENU
	else if (currentMenuState == MAIN_MENU )
	{
		//Start
		if (sym == 13 && mainMenuOptionSelection == NEW_GAME)
		{
			OnInitialize();
			StartGame();
			gameStarted = true;
			mainBgMusic.Play("mainBg.wav");
			mainBgMusic.SetVolume(20);
			currentMenuState = IN_GAME;
		}

		//SHOW CONTROLERS
		if (sym == 13 && mainMenuOptionSelection == CONTROLS)
		{
			currentMenuState = SHOW_CONTROLLERS;
		}

		//Exit
		if (sym == 13 && mainMenuOptionSelection == EXIT)
		{
			StopGame();
		}

		//Menu Scroller
		if ((sym == SDLK_s) || (sym == SDLK_DOWN))
		{
			mainMenuOptionSelection++;
			if (mainMenuOptionSelection > 2)  mainMenuOptionSelection = 0;
		}
		else if ((sym == SDLK_w) || (sym == SDLK_UP))
		{
			mainMenuOptionSelection--;
			if (mainMenuOptionSelection < 0)  mainMenuOptionSelection = 2;
		}
	}
}


void CMyGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{
	currentMousePos = ScreenToFloorCoordinate(x, y);
}