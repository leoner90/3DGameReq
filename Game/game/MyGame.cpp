#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/Enemy.h"
#include "headers/Map.h"

//globall for now
std::vector<Enemy*> AllEnemies; // to make proper enemies init

//*************** INIT ***************
void CMyGame::OnInitialize()
{
	cameraMovement = false;
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
		i +=   rand() % 350;
		z +=   rand() % 350;
		enemyType = !enemyType;
	}

	//make sure that sld handels all the input not a window
	//SDL_WM_GrabInput(SDL_GRAB_ON);

	EnableFog();

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
		
		enemy->OnUpdate(GetTime(), *player , *map);

		//* if regular enemie dead -> delete;
		if (enemy->isDead) {
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
	player->OnDraw(g);
	//font.DrawNumber(100, 100, camera.rotation.x, CColor::White(), 52);
 
	


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

	//ShowBoundingBoxes();
	//ShowCoordinateSystem();
}

//*************** CAMERA ***************
void CMyGame::CameraControl(CGraphics* g)
{

	 
 

	// ------ Global Transformation Functions -----
	glScalef(world.scale, world.scale, world.scale);  // scale the game world
	glTranslatef(world.position.x, world.position.y, world.position.z);  // translate game world

	glTranslatef(0, 0, camera.position.z);  // translate game world in z-axis direction
	glRotatef(camera.rotation.x + 50, 1, 0, 0);	// rotate game world around x-axis
	glRotatef(camera.rotation.y, 0, 1, 0);	// rotate game world around x-axis

	// -----  Player camera control ----------

	// rotate game world with player and face forward
	//glRotatef(-player->playerModel.GetRotation() + 90, 0, 1, 0);

	// move camera up from player position. Camera will rise with the player
	glTranslatef(0, -player->playerModel.GetY() - camera.position.y, 0);

	// draw the skydome before game world is translated
   // it makes the skydome stationary
	skydome.Draw(g);
	// position game world at the player position (excluding skydome)
	glTranslatef(-player->playerModel.GetX(), 0, -player->playerModel.GetZ());

	UpdateView();
	Light.Apply();

	//camera Zoom
	if (IsKeyDown(SDLK_KP_PLUS))  if (camera.position.z < 2000) camera.position.z += 20;  // camera distance
	if (IsKeyDown(SDLK_KP_MINUS)) if (camera.position.z > 0)camera.position.z -= 20;  // camera distance
 
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

	// Load skydome geometry and texture
	skydome.LoadModel("Skydome/Skydome.obj", "Skydome/Skydome.bmp");
	skydome.SetScale(200);  // scale to desired size
	skydome.SetY(-300.0f); // move down to avoid seeing the base
}


//FOG
void CMyGame::EnableFog()
{
	GLuint filter;                      // Which Filter To Use
	GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
	GLuint fogfilter = 1;                    // Which Fog To Use
	GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };      // Fog Color

	//fog settings
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);          // We'll Clear To The Color Of The Fog ( Modified )

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);        // Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.03f);              // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
	glFogf(GL_FOG_START, 0.1f);             // Fog Start Depth
	glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
	glEnable(GL_FOG);                   // Enables GL_FOG

}


//*************** KEYBOARD EVENTS ***************



void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Player KeyBoard Controller
	player->OnKeyDown(sym , currentMousePos);

	//Start
	MainMenyController(sym);


}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	player->OnLButtonDown(ScreenToFloorCoordinate(x, y), currentMousePos, GetTime());
}



void CMyGame::MaiMenuDraw(CGraphics* g)
{
	if (currentMenuState == SHOW_CONTROLLERS) 
	{
		mainMenushowControlers.Draw(g);
		font.DrawText((float)Width / 2 - 100, (float)camera.rotation.x  / 2 + 280, "CONTROLS", CColor::White(), 52);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 200, "LEFT MOUSE BTN. - SHOOT", CColor::White() , 32);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 150, "W / S / A / D  - MOVE CHARACTER", CColor::White(), 32);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 100, "Q - USE SKILL", CColor::White(), 32);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 50, "MOUSE WHEEL - CHANGE SKILL", CColor::White(), 32);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 , "E - INTERACT", CColor::White(), 32);

		font.DrawText((float)Width / 2 - 70.f, 100, "BACK", CColor::White(), 42);
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


	if (cameraMovement)
	{
		float addX = cameraControlMouseInitPose.GetX() - x;
		float addY = cameraControlMouseInitPose.GetY() - y;

		//if(camera.rotation.x - addY > -40 && camera.rotation.x + addY < 30)
		camera.rotation.x += addY / 10;
		camera.rotation.y -= addX / 10;
	}
}


void CMyGame::OnWheelUp(Uint16 x, Uint16 y)
{
	player->playerOnWheelUp();
}

void CMyGame::OnWheelDown(Uint16 x, Uint16 y)
{
	player->playerOnWheelDown();
}

void CMyGame::OnMButtonDown(Uint16 x, Uint16 y)
{
	cameraMovement = true;
	cameraControlMouseInitPose.SetX(x);
	cameraControlMouseInitPose.SetY(y);
}

void CMyGame::OnMButtonUp(Uint16 x, Uint16 y)
{
	cameraMovement = false;
}
