#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/Enemy.h"
#include "headers/Map.h"
#include "headers/Shop.h"
#include "headers/UIDialogBox.h"
 

//globall for now
std::vector<Enemy*> AllEnemies; // to make proper enemies init

//*************** INIT ***************
void CMyGame::OnInitialize()
{
	cameraMovement = false;
	currentMenuState = MAIN_MENU;
	mainMenuOptionSelection = NEW_GAME;
	gameStarted = false;
	deathScreenTimer = 0;
	enemyOneSpawnDelay = enemyTwoSpawnDelay = 0;

	// Main Objects
	delete map;
	map = new Map();
	
	//shop
	delete shop;
	shop = new Shop();

	delete player;
	player = new Player();

	delete playerInterface;
	playerInterface = new PlayerInterface();

	delete dialogBox;
	dialogBox = new UIDialogBox();
 

	//music
	mainBgMusic.Stop();
	mainBgMusic.Play("mainBg.wav");
	mainBgMusic.SetVolume(35);

	//StartScreen Models
	InitSpritesAndModels();

	//Main inits
	player->init();
	map->init();
	
	//Shop
	shop->init((float)Width, (float)Height);

	playerInterface->init(Width,Height);

	Light.Enable();
	font.LoadDefault();

	dialogBox->init((float)Width, (float)Height);
	
	//clear all enemies
	for (auto enemy : AllEnemies) {
		delete enemy;
	}
	AllEnemies.clear();




	//make sure that sld handels all the input not a window
	//SDL_WM_GrabInput(SDL_GRAB_ON);

	EnableFog();

	totalEnemiesOnHold = 0;
	totalEnemiesToSpawn = 5;

}

void CMyGame::enemySpawn()
{
	enum allEnemies{EN2, OGRO};

	float randomSpawnPoint = rand() % 100 + 50;

	//Enemy ONe
	if (enemyOneSpawnDelay < GetTime())
	{
		//use Clone instead
		AllEnemies.push_back(new Enemy());
		AllEnemies.back()->init(3800 + randomSpawnPoint, 100, 400 + randomSpawnPoint, OGRO, *map);
		enemyOneSpawnDelay = GetTime() + 3000 + rand() % 3000;
		totalEnemiesOnHold++;
	}


	//EnemyTwo
	if (enemyTwoSpawnDelay < GetTime())
	{
		AllEnemies.push_back(new Enemy());
		AllEnemies.back()->init(2500 + randomSpawnPoint, 100, 700 + randomSpawnPoint, EN2, *map);
		enemyTwoSpawnDelay = GetTime() + 2000 + rand() % 2000;
		totalEnemiesOnHold++;
	}


	//start a wave
	int howManyEnemiesWasOnhold = 0;
	if (totalEnemiesOnHold == totalEnemiesToSpawn + rand() % 5)
	{
		
		for (auto enemy : AllEnemies)
		{
			if (enemy->OnSpawnHold) howManyEnemiesWasOnhold++;
			enemy->OnSpawnHold = false;
			totalEnemiesOnHold = 0;
			totalEnemiesToSpawn += 3;
		}

		dialogBox->showBox(0, 1, 1, 3000); // speaker id , text id , priority

		if (howManyEnemiesWasOnhold == totalEnemiesToSpawn)
		{
			
		}
	}
	


}



//*************** UPDATE ***************
void CMyGame::OnUpdate() 
{
	//Death Handler Move To function
	if ((player->isPlayerDead) && gameStarted)
	{
		currentMenuState = DEATHSCREEN;
		gameStarted = false;
		SetGameMode(MODE_MENU);
		deathScreenTimer = GetTime() + 3000;
	} 
	else if(player->isPlayerDead && deathScreenTimer <= GetTime())
	{
		currentMenuState = MAIN_MENU;
	}

	//------


	

	if (IsMenuMode() || IsGameOver() || IsPaused() || currentMenuState == MAIN_MENU)
	{
		return;
	}
		
	else 
	{
		enemySpawn();
		map->OnUpdate(GetTime());
		player->OnUpdate(GetTime(), IsKeyDown(SDLK_d), IsKeyDown(SDLK_a), IsKeyDown(SDLK_w), IsKeyDown(SDLK_s), *map, AllEnemies);
		playerInterface->OnUpdate(GetTime(), map->portal.GetHealth(), *player, *dialogBox);

		//Shop
		shop->OnUpdate(GetTime(), *player, *dialogBox);

		//dialog box
		dialogBox->OnUpdate(GetTime());

		//Enemies delete
		int i = 0;
		for (auto enemy : AllEnemies) {

			enemy->OnUpdate(GetTime(), *player, *map, AllEnemies, i);

			//* if regular enemie dead -> delete;
			if (enemy->isDead) {
				AllEnemies.erase(AllEnemies.begin() + i);
				delete enemy;
			}

			i++;
		}
	}



}

//*************** 2D RENDER ***************
void CMyGame::OnDraw(CGraphics* g)
{
	if (IsMenuMode())
	{
		if (currentMenuState == MAIN_MENU) MaiMenuDraw(g);
		else if (currentMenuState == CHAR_STATS) CharStatsDraw(g);
		else if (currentMenuState == SHOP) shop->openShop(g);
		else if (currentMenuState == DEATHSCREEN) 	deathScreen.Draw(g);
		return;
	}

	playerInterface->OnDraw(g);
	player->OnDraw(g);
	map->OnDraw(g);
	//Shop
	shop->OnDraw(g);
	

	for (auto enemy : AllEnemies) 
	{
		enemy->OnDraw(g, WorldToScreenCoordinate(enemy->enemyModel.GetPositionV()));
	}

	dialogBox->OnDraw(g);
}

//*************** 3D RENDER ***************
void CMyGame::OnRender3D(CGraphics* g)
{ 
	if (IsMenuMode() || IsGameOver())
		return;
	CameraControl(g);
	map->OnRender3D(g);
	player->OnRender3D(g, world);
	

	for (auto enemy : AllEnemies) {
		enemy->OnRender3D(g);
	}

	//Shop
	shop->OnRender3D(g);

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



//INIT SPRITES
void CMyGame::InitSpritesAndModels()
{
	// Main Menu screen
	startScreen.LoadImage("startScreen.jpg");
	startScreen.SetSize((float)Width, (float)Height);
	startScreen.SetPosition((float)Width / 2, (float)Height / 2);

	//Main Menu Controllers
	mainMenushowControlers.LoadImage("mainMenushowControlers.jpg");
	mainMenushowControlers.SetSize((float)Width, (float)Height);
	mainMenushowControlers.SetPosition((float)Width / 2, (float)Height / 2);

	// char screen
	CharStatsMenu.LoadImage("charStats.jpg");
	CharStatsMenu.SetSize((float)Width, (float)Height);
	CharStatsMenu.SetPosition((float)Width / 2, (float)Height / 2);

	// Load skydome geometry and texture
	skydome.LoadModel("Skydome/Skydome.obj", "Skydome/Skydome.bmp");
	skydome.SetScale(200);  // scale to desired size
	skydome.SetY(-300.0f); // move down to avoid seeing the base

	//deathScreen
	deathScreen.LoadImage("deathScreen.jpg");
	deathScreen.SetSize((float)Width, (float)Height);
	deathScreen.SetPosition((float)Width / 2, (float)Height / 2);
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


//*************** Main Menu Handler
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

void CMyGame::CharStatsDraw(CGraphics* g)
{
	CharStatsMenu.Draw(g);
	//damage
	font.DrawText((float)Width / 2, (float)Height - 200, "DAMAGE", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 200, player->playerDamage, CColor::White(), 18);

	//hp
	font.DrawText((float)Width / 2, (float)Height -250 , "HEALTH", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 250, player->playerCurrentHp , CColor::White(), 18);
	font.DrawText((float)Width / 2 + 120, (float)Height - 250, " / ", CColor::White(), 18);
	font.DrawNumber((float)Width /2 + 140, (float)Height - 250, player->playerMaxHp, CColor::White(), 18);

	//armor
	font.DrawText((float)Width / 2, (float)Height - 300, "ARMOR", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 300, player->playerCurrentArmor, CColor::White(), 18);
	font.DrawText((float)Width / 2 + 120, (float)Height - 300, " / ", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 140, (float)Height - 300, player->playerMaxArmor, CColor::White(), 18);

	//ENERGY
	font.DrawText((float)Width / 2, (float)Height - 350, "ENERGY", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 350, player->CurrentEnergy, CColor::White(), 18);
	font.DrawText((float)Width / 2 + 120, (float)Height - 350, " / ", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 140, (float)Height - 350, player->maxEnergy, CColor::White(), 18);
	
	//resources
	font.DrawText((float)Width / 2, (float)Height - 400, "ARMOR PARTS:", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 400, player->armorComponents, CColor::White(), 18);

	font.DrawText((float)Width / 2, (float)Height - 450, "WEAPON  PARTS:", CColor::White(), 18);
	font.DrawNumber((float)Width / 2 + 100, (float)Height - 450, player->weaponComponents, CColor::White(), 18);

	font.DrawText((float)Width / 2, (float)Height - 500, "SKILLS:", CColor::White(), 18);
}


//*************** KEYBOARD EVENTS ***************



void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Player KeyBoard Controller
	if (!IsMenuMode()) player->OnKeyDown(sym, currentMousePos);

	//Start
	MainMenyController(sym);


}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	if (IsMenuMode())  shop->OnLButtonDown(x, y);
	if (!IsMenuMode()) player->OnLButtonDown(ScreenToFloorCoordinate(x, y), currentMousePos, GetTime());

	 if (IsMenuMode() && !shop->isPlayerShoping && gameStarted &&   currentMenuState == SHOP)
	{
		ResumeGame();
		SetGameMode(MODE_RUNNING);
		currentMenuState = IN_GAME;
		shop->isPlayerShoping = false;
	}
}


void CMyGame::MainMenyController(SDLKey sym)
{
 
	// Call / Cancel Menu on ESC btn. pressed
	if (!IsMenuMode()  && gameStarted )
	{
		if (sym == SDLK_ESCAPE)
		{
			PauseGame();
			SetGameMode(MODE_MENU);
			currentMenuState = MAIN_MENU;
		}
		else if (sym == SDLK_c)
		{
			//PauseGame();
			SetGameMode(MODE_MENU);
			currentMenuState = CHAR_STATS;
		}

		else if (sym == SDLK_e && shop->shopIsInRange)
		{
			PauseGame();
			SetGameMode(MODE_MENU);
			currentMenuState = SHOP;

		}
		
	}
	else if (IsMenuMode() && sym == SDLK_ESCAPE  && gameStarted && (currentMenuState == MAIN_MENU || currentMenuState == CHAR_STATS || currentMenuState == SHOP) )
	{

		ResumeGame();
		SetGameMode(MODE_RUNNING);
		for (auto enemy : AllEnemies) {
			enemy->enemyModel.ResetTime();
		}
		currentMenuState = IN_GAME;
		shop->isPlayerShoping = false;
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

	player->OnMouseMove(currentMousePos);
	if (cameraMovement)
	{
		float addX = cameraControlMouseInitPose.GetX() - x;
		float addY = cameraControlMouseInitPose.GetY() - y;

		camera.rotation.x += addY / 10;
		camera.rotation.y -= addX / 10;
	}
}


void CMyGame::OnWheelUp(Uint16 x, Uint16 y)
{
	if(!IsMenuMode()) player->playerOnWheelUp();
}

void CMyGame::OnWheelDown(Uint16 x, Uint16 y)
{
	if (!IsMenuMode()) player->playerOnWheelDown();
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
