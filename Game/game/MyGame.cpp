#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/Enemy.h"
#include "headers/Map.h"
#include "headers/Shop.h"
#include "headers/UIDialogBox.h"
#include "headers/Cutscene.h"
#include "headers/loadingScreen.h"
#include "headers/Portal.h"
#include "headers/Radar.h"


CMyGame::CMyGame()
{
	HideMouse();
	YcameraInitRotation = camera.rotation.y;
	ZcameraInitState = camera.position.z;
	XcameraInitState = camera.position.x;
	YcameraInitState = camera.position.x;
}
//*************** INIT ***************
void CMyGame::OnInitialize()
{
	HideMouse();
	cameraMovement = false;
	currentMenuState = MAIN_MENU;
	mainMenuOptionSelection = NEW_GAME;
	SetGameMode(MODE_PAUSED);

	localW = (float)Width;
	localH = (float)Height;
	//Main Objects
	map = new Map();
	portal = new Portal(localW, localH);
	shop = new Shop(localW, localH);
	cutscene = new Cutscene(localW, localH);
	player = new Player();
	playerInterface = new PlayerInterface(localW, localH);
	dialogBox = new UIDialogBox(localW, localH);
	loadingScreen = new LoadingScreen(localW, localH);
	radar = new Radar(localW, localH);
	//loading screen 
	loadingScreen->init();

	//music
	mainBgMusic.Play("mainBg.wav");
	mainBgMusic.SetVolume(35);

	//StartScreen Models
	InitSpritesAndModels();
	Light.Enable();
	font.LoadDefault();
	EnableFog();
	//make sure that sld handels all the input not a window //SDL_WM_GrabInput(SDL_GRAB_ON);

	
	//enemies
	enemyModelOne = new CModelMd3();
	enemyModelTwo = new CModelMd3();
	boss = new CModelMd3();
	enemyModelOne->LoadModel("enemies/mon01.md3");
	enemyModelOne->LoadTexture("enemies/mon01.png");
	enemyModelOne->SetScale(25.5f);

	enemyModelTwo->LoadModel("enemies/65.md3");
	enemyModelTwo->SetScale(15.5f);
	enemyModelTwo->LoadTexture("enemies/65.png");

	boss->LoadModel("enemies/boss02.md3");
	boss->SetScale(30.5f);
	boss->LoadTexture("enemies/boss2.png");
}



//ON NEW GAME START
void CMyGame::OnStartLevel(int level)
{
	map->init();

	shop->init(localW, localH); //Shop init
	playerInterface->init(localW, localH); //player Interface init
	dialogBox->init(); // dialog Box init
	portal->init();
	radar->init();
	cutscene->init(localW, localH, *player); 	//cutscene
	player->init();//Main inits

	//****ENEMIES
	for (auto enemy : AllEnemies) delete enemy;
	AllEnemies.clear();

	totalEnemiesOnHold = 0;
	totalEnemiesToSpawn = 4;
	InitSpawnDelay = 60000 *  0.3; // spawn start , not the wave
	enemyOneSpawnDelay = enemyTwoSpawnDelay = 0; // init SPAWN DELAY -> set In Enemy Spawn fun!!!!!!
	//****

	//resets
	gameStarted = false;
	camera.rotation.y = YcameraInitRotation;
	camera.position.z = ZcameraInitState;
	camera.position.x = XcameraInitState;
	camera.position.y = YcameraInitState;

	SetGameMode(MODE_PAUSED);
	deathScreenTimer = 0;
	isBossSpawn = false;
	
	rainBgEffect.Pause();
}




//*************** UPDATE ***************
void CMyGame::OnUpdate() 
{
	Uint32 t = (float)GetTime();
	//portal Charged
	if (portal->isPortalCharged && currentMenuState != CUTSCENE)
	{
		player->footsteps.Pause(); // bug with a speed
		cutscene->startCutscene(1);
		currentMenuState = CUTSCENE;
	}

	//end Of the game Win
	if (cutscene->curentCutSceneNum == 1 && !cutscene->isCutscenePlaying)
	{
		currentMenuState = MAIN_MENU;
		SetGameMode(MODE_PAUSED);
		gameStarted = false;
	}


	//CUTSCENE
	if (!cutscene->isCutscenePlaying && currentMenuState == CUTSCENE && cutscene->curentCutSceneNum == 0)
	{
		currentMenuState = IN_GAME;
		camera.rotation.y = YcameraInitRotation;
		camera.position.z = 650;
		rainBgEffect.Play("rain.wav", -1);
		rainBgEffect.SetVolume(60);
	}

	if (currentMenuState == CUTSCENE && gameStarted)
	{
		map->OnUpdate(t, *player);
		bool fullWidth = true;
		dialogBox->OnUpdate(t, fullWidth);
		cutscene->Update(t, *dialogBox);
		return;
	}

	//Death Handler Move To function
	if ((player->isPlayerDead) && gameStarted)
	{
		currentMenuState = DEATHSCREEN;
		gameStarted = false;
		SetGameMode(MODE_MENU);
		deathScreenTimer = t + 3000;
	} 
	else if(player->isPlayerDead && deathScreenTimer <= t)
	{
		currentMenuState = MAIN_MENU;
		SetGameMode(MODE_PAUSED);
		gameStarted = false;
	}

	//LOADING_SCREEN 
	if(currentMenuState == LOADING_SCREEN) loadingScreen->Update(t);



	//RETURN IF MENU MODE
	if ( IsGameOver() || IsPaused() || IsMenuMode() || currentMenuState != IN_GAME) return; // IN GAME , when exit the game mode == exit rising bugs
	

	//Enemy spawn
	enemySpawn();

	//Objects updates
	map->OnUpdate(t, *player);
	portal->OnUpdate(t, *dialogBox);
	bool fullWidth = false;
	dialogBox->OnUpdate(t, fullWidth);
	player->OnUpdate(t, IsKeyDown(SDLK_d), IsKeyDown(SDLK_a), IsKeyDown(SDLK_w), IsKeyDown(SDLK_s), *map, AllEnemies, currentMousePos, *portal, WorldToScreenCoordinate(player->playerModel.GetPositionV()));
	playerInterface->OnUpdate(t, *player, *dialogBox);
	shop->OnUpdate(t, *player, *dialogBox);
	radar->OnUpdate(t, AllEnemies, *player);
	
 

	//Enemies delete

	for (auto enemy : AllEnemies) {
		//o = enemy todo
		enemy->OnUpdate(GetTime(), *player, *map, AllEnemies, 0, *portal);

		//* if regular enemie dead -> delete;
		if (enemy->isDead) 
		{
			delete enemy;
			AllEnemies.erase(find(AllEnemies.begin(), AllEnemies.end(), enemy));
		}	
	}
}

//*************** 2D RENDER ***************
void CMyGame::OnDraw(CGraphics* g)
{
	// MENU MODE
	if (IsPaused() || IsMenuMode())
	{
		if (currentMenuState == MAIN_MENU || currentMenuState == SHOW_CONTROLLERS) MaiMenuDraw(g);
		else if (currentMenuState == CHAR_STATS) CharStatsDraw(g);
		else if (currentMenuState == SHOP) shop->openShop(g);
		else if (currentMenuState == DEATHSCREEN) deathScreen.Draw(g);
		else if (currentMenuState == LOADING_SCREEN) loadingScreen->OnDraw(g);
		return;
	}

	// CUTSCENE
	if (currentMenuState == CUTSCENE && gameStarted)
	{
		cutscene->Draw2d(g);
		dialogBox->OnDraw(g);
		map->OnDraw(g);
		return;
	}

	//IN GAME
	if(currentMenuState != IN_GAME) return; // IN GAME , when exit the game mode == exit rising bugs

	mousePointer.Draw(g);
	player->OnDraw(g, *dialogBox, *shop);
	playerInterface->OnDraw(g);
	shop->OnDraw(g);
	portal->OnDraw(g);
	dialogBox->OnDraw(g);
	radar->OnDraw(g);
	for (auto enemy : AllEnemies) enemy->OnDraw(g, WorldToScreenCoordinate(enemy->enemyModel->GetPositionV()));
}

//*************** 3D RENDER ***************
void CMyGame::OnRender3D(CGraphics* g)
{ 
	if (IsGameOver() || IsPaused() || IsMenuMode()) return; // IN GAME , when exit the game mode == exit rising bugs

	//CUTSCENE
	if (currentMenuState == CUTSCENE && gameStarted)
	{
		CameraControl(g);
		map->OnRender3D(g);
		cutscene->Draw3d(g);
		portal->OnRender3D(g);
		return;
	}

	//IN GAME
	if(currentMenuState != IN_GAME) return;

	CameraControl(g);
	map->OnRender3D(g);
	player->OnRender3D(g, world);
	portal->OnRender3D(g);
	for (auto enemy : AllEnemies) enemy->OnRender3D(g);
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

	glTranslatef(0, 0, camera.position.z  );  // translate game world in z-axis direction
	glRotatef(camera.rotation.x + 50, 1, 0, 0);	// rotate game world around x-axis
	if (currentMenuState == CUTSCENE && gameStarted )
		camera.rotation.y -= cutscene->shiprotationalAngelY / 2;
	//camera.rotation.y -= cutscene->shiprotationalAngelY / 2;
	
	glRotatef(camera.rotation.y , 0, 1, 0);	// rotate game world around x-axis

	// rotate game world with player and face forward //glRotatef(-player->playerModel.GetRotation() + 90, 0, 1, 0);
	// move camera up from player position. Camera will rise with the player
	if (currentMenuState == CUTSCENE && gameStarted )
		glTranslatef(0, -cutscene->cutcceneCameraPosition.GetY() - camera.position.y , 220);
	else
		glTranslatef(0, -player->playerModel.GetY() - camera.position.y, 0);

   // it makes the skydome stationary / draw the skydome before game world is translated
	skydome.Draw(g);
	
	// position game world at the player position (excluding skydome)
	if (currentMenuState == CUTSCENE && gameStarted )
		glTranslatef(-cutscene->cutcceneCameraPosition.GetX(), 700, -cutscene->cutcceneCameraPosition.GetZ());
	else
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
	CharStatsMenu.LoadImage("charStats.png");
	CharStatsMenu.SetSize((float)Width, (float)Height);
	CharStatsMenu.SetPosition((float)Width / 2, (float)Height / 2);

	// Load skydome geometry and texture
	skydome.LoadModel("Skydome/Skydome.obj", "Skydome/Skydome.bmp");
	skydome.SetScale(400);
	skydome.SetY(-300.0f);

	//deathScreen
	deathScreen.LoadImage("deathScreen.jpg");
	deathScreen.SetSize((float)Width, (float)Height);
	deathScreen.SetPosition((float)Width / 2, (float)Height / 2);

	//mouse Pointer 
	mousePointer.LoadImage("mousePointer.png");
	mousePointer.SetSize(20, 20);
}


//FOG
void CMyGame::EnableFog()
{
	GLuint filter;                      // Which Filter To Use
	GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
	GLuint fogfilter = 1;                    // Which Fog To Use
	GLfloat fogColor[4] = { 0.5f, 0.6f, 0.5f, 1.0f };      // Fog Color

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
		font.DrawText((float)Width / 2 - 100,	(float)Height / 2 + 280, "CONTROLS", CColor::White(), 52);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 200, "LEFT MOUSE BTN. - SHOOT", CColor::White() , 22);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 150, "W / S / A / D  - MOVE CHARACTER", CColor::White(), 22);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 100, "Q - USE SKILL", CColor::White(), 22);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 + 50, "MOUSE WHEEL - CHANGE SKILL", CColor::White(), 22);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 , "E - INTERACT", CColor::White(), 22);
		font.DrawText((float)Width / 2 - 220.f, (float)Height / 2 - 50, "C - CHARACTER STATS", CColor::White(), 22);
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

	//lvl
	font.DrawNumber(686, (float)Height - 468, 1, CColor::White(), 38);

	//exp
	font.DrawNumber(686, (float)Height - 555, player->currentExp, CColor::White(), 38);
	//damage
	font.DrawNumber(686, (float)Height - 650, player->playerDamage, CColor::White(), 38);
	
	//rateOf Fire
	font.DrawNumber(686, (float)Height - 770, player->shootingDelay, CColor::White(), 38);
	
	//energy Charge rate regen
	font.DrawNumber(686, (float)Height - 880, player->energyRegenPerSec, CColor::White(), 38);

	//ENERGY
	font.DrawNumber(1493, (float)Height - 468, player->CurrentEnergy, CColor::White(), 38);
	font.DrawText(1493 + 50, (float)Height - 468, " / ", CColor::White(), 38);
	font.DrawNumber(1493 + 100, (float)Height - 468, player->maxEnergy, CColor::White(), 38);


	//HP
	font.DrawNumber(1493 , (float)Height - 650, player->playerCurrentHp , CColor::White(), 38);
	font.DrawText(1493 + 50, (float)Height - 650, " / ", CColor::White(), 38);
	font.DrawNumber(1493 + 100, (float)Height - 650, player->playerMaxHp, CColor::White(), 38);

	//Armor
	font.DrawNumber(1493, (float)Height - 770, player->playerCurrentArmor, CColor::White(), 38);
	font.DrawText(1493 + 50, (float)Height - 770, " / ", CColor::White(), 38);
	font.DrawNumber(1493 + 100, (float)Height - 770, player->playerMaxArmor, CColor::White(), 38);

	//Armor
	font.DrawNumber(1493, (float)Height - 880, player->armorRegenPerSec, CColor::White(), 38);
;
	
}


void CMyGame::enemySpawn()
{
	
	enum allEnemiesTypes { EN2, OGRO, BOSS };

	float randomSpawnPoint = rand() % 100 + 50;
	if (enemyOneSpawnDelay == 0)
	{
		enemyOneSpawnDelay = enemyTwoSpawnDelay = GetTime() + InitSpawnDelay;
	}

	//4 spawn points
	CVector spawnPos[4] =
	{
		CVector(5500 + randomSpawnPoint,100,5400 + randomSpawnPoint),
		CVector(5500 + randomSpawnPoint,100,-5400 + randomSpawnPoint),
		CVector(-5500 + randomSpawnPoint,100,-5400 + randomSpawnPoint),
		CVector(-5500 + randomSpawnPoint,100,5400 + randomSpawnPoint),
	
	};

	//EnemyOne
	if (enemyOneSpawnDelay < GetTime())
	{
		 
		AllEnemies.push_back(new Enemy());
		AllEnemies.back()->init(spawnPos[rand() % 4], OGRO, *map, *portal, *enemyModelTwo);
		enemyOneSpawnDelay = GetTime() + 10000 + rand() % 3000;
		totalEnemiesOnHold++;
	}

	//EnemyTwo
	if (enemyTwoSpawnDelay < GetTime())
	{
		AllEnemies.push_back(new Enemy());
		AllEnemies.back()->init(spawnPos[rand() % 4], EN2, *map, *portal, *enemyModelOne);
		enemyTwoSpawnDelay = GetTime() + 10000 + rand() % 2000;
		totalEnemiesOnHold++;
	}

	//boss
	if (portal->portalChargeInPercent > 75 && !isBossSpawn)
	{
		bossSpawnSound.Play("boss.wav");
		bossSpawnSound.SetVolume(30);
		isBossSpawn = true;
		AllEnemies.push_back(new Enemy());
		AllEnemies.back()->init(spawnPos[rand() % 4], BOSS, *map, *portal, *boss);
		AllEnemies.back()->OnSpawnHold = false;
	}



	//start a wave
	int howManyEnemiesWasOnhold = 0; // if not enought -> dont show alert

	if (totalEnemiesOnHold == totalEnemiesToSpawn)
	{

		for (auto enemy : AllEnemies)
		{
			if (enemy->OnSpawnHold) howManyEnemiesWasOnhold++;
			enemy->OnSpawnHold = false;
			totalEnemiesOnHold = 0;
		}
		totalEnemiesToSpawn += 2;
		dialogBox->showBox(1, 9,9, 1, 3000); // speaker id , text id , priority
	}
}

//*************** KEYBOARD EVENTS ***************



void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Player KeyBoard Controller
	if (!IsPaused()) player->OnKeyDown(sym, currentMousePos);

	if (currentMenuState == SHOP) shop->OnKeyDown(sym);
	//Start
	MainMenuController(sym);
}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	
	//player Left Btn
	if (!IsPaused() && currentMenuState == IN_GAME)
		player->OnLButtonDown(ScreenToFloorCoordinate(x, y), currentMousePos, GetTime());

	//If shopping (but armor/damage)
	if (IsPaused() && currentMenuState == SHOP)  shop->OnLButtonDown(x, y);

	//SHOP X BTN EXIT
	if (IsPaused() && !shop->isPlayerShoping && currentMenuState == SHOP)
	{
		ResumeGame();
		SetGameMode(MODE_RUNNING);
		currentMenuState = IN_GAME;
		shop->isPlayerShoping = false;
		HideMouse();
	}
}


void CMyGame::MainMenuController(SDLKey sym)
{
	// Call / Cancel Menu on ESC btn. pressed
	if (!IsPaused() && gameStarted && currentMenuState == IN_GAME)
	{
		if (sym == SDLK_ESCAPE)
		{
			player->footsteps.Pause();// bug with a speed
			SetGameMode(MODE_PAUSED);
			currentMenuState = MAIN_MENU;
		}
		else if (sym == SDLK_c)
		{
			player->footsteps.Pause();// bug with a speed
			SetGameMode(MODE_PAUSED);
			currentMenuState = CHAR_STATS;
		}

		else if (sym == SDLK_e && shop->shopIsInRange)
		{
			player->footsteps.Pause(); // bug with a speed
			SetGameMode(MODE_PAUSED);
			currentMenuState = SHOP;
			ShowMouse();
		}
	}
	else if (sym == SDLK_ESCAPE && gameStarted && currentMenuState == CUTSCENE)
	{
		cutscene->dialogNumber = 11; //skip dialog
	}

	else if (IsPaused() && sym == SDLK_ESCAPE  && gameStarted && (currentMenuState == MAIN_MENU || currentMenuState == CHAR_STATS || currentMenuState == SHOP) )
	{
		HideMouse();
		SetGameMode(MODE_RUNNING);
		currentMenuState = IN_GAME;
		shop->isPlayerShoping = false;
	}


	//LOADING COMPLETED
	if (sym == 13 && currentMenuState == LOADING_SCREEN && loadingScreen->loadingCompleted)
	{
		//OnStartLevel(1);
		StartGame();
		gameStarted = true;
		cutscene->startCutscene(0);
		currentMenuState = CUTSCENE;
	}


	////******* IF NOT IN MENU MODE -> RETURN~~~~~~~~~~~~~~~~~~~~~~
	if (!IsPaused()) return;

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
			OnStartLevel(1);
			SetGameMode(MODE_MENU); // to update loading sprite
			loadingScreen->init();
			loadingScreen->loadingStarted = true;
			currentMenuState = LOADING_SCREEN;
			
			
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
	mousePointer.SetPosition(x, y);

	//player rotation to mouse pos
	if(currentMenuState == IN_GAME && !player->playerPreDeahAnimation) player->OnMouseMove(currentMousePos);

	//camera rotation
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
	if(!IsMenuMode() && !IsPaused()) player->playerOnWheelUp();
}

void CMyGame::OnWheelDown(Uint16 x, Uint16 y)
{
	if (!IsMenuMode() && !IsPaused()) player->playerOnWheelDown();
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


void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{
	player->OnRButtonDown(GetTime());
}

void CMyGame::OnRButtonUp(Uint16 x, Uint16 y)
{
	player->OnRButtonUp();
}