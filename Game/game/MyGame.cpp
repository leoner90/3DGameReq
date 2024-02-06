#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/Enemy.h"
#include "headers/Map.h"

Map* map = new Map();
Player* player = new Player();

//*************** INIT ***************
void CMyGame::OnInitialize()
{
	gameOption = false;
	player->init();
	font.LoadDefault(); 
	Light.Enable();
	InitSpritesAndModels();
}

//*************** UPDATE ***************
void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) return;
	player->OnUpdate(gameOption, GetTime(), IsKeyDown(SDLK_d) , IsKeyDown(SDLK_a) , IsKeyDown(SDLK_w), IsKeyDown(SDLK_s));
    if (hbar.GetHealth() <= 0) GameOver();

	//Map 2d
	map2dExample.Update(GetTime());
}

//*************** 2D RENDER ***************
void CMyGame::OnDraw(CGraphics* g)
{
	if (IsMenuMode())
	{
	 startScreen.Draw(g);
	 return;
	}

	hbar.Draw(g);

	if (!gameOption) 
	{
	
		
		player->OnDraw(g);
		
	}
}

//*************** 3D RENDER ***************
void CMyGame::OnRender3D(CGraphics* g)
{ 
	map2dExample.Draw(g);
	
	if (gameOption)
	{
		CameraControl(g);
		floor.Draw(g);
		player->OnRender3D(g, world);
	}

	//ShowBoundingBoxes();
	//ShowCoordinateSystem();
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
	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	// start screen
	startScreen.LoadImage("startScreen.jpg");
	startScreen.SetSize(1920,1080);
	startScreen.SetPosition((float)Width / 2, (float)Height / 2);

	// set size of the game world
	floor.SetSize(9000, 9000);

	// setup healthbar
	hbar.SetSize(100, 15);
	hbar.SetPosition(1820, 1050);
	hbar.SetHealth(100);

	//2Dmap
	map2dExample.LoadImage("Map/mainbg.jpg");
	map2dExample.SetSize(4000, 4000);
	map2dExample.SetPosition(0 , 0);
}

//*************** KEYBOARD EVENTS ***************
void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Exit
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();

	//Restart
	if (sym == SDLK_F1) {
		gameOption = false;
		StartGame();
	}
	if (sym == SDLK_F2) {
		gameOption = true;
		StartGame();
	}
}
