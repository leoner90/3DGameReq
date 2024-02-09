#include "headers/MyGame.h"
#include "headers/Player.h"
#include "headers/PlayerInterface.h"
#include "headers/Enemy.h"
#include "headers/Map.h"



//*************** INIT ***************
void CMyGame::OnInitialize()
{

	// Main Objects
	map = new Map();
	player = new Player();
	playerInterface = new PlayerInterface();
	enemy = new Enemy();

	//music
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
}

//*************** UPDATE ***************
void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) return;
	player->OnUpdate(GetTime(), IsKeyDown(SDLK_d) , IsKeyDown(SDLK_a) , IsKeyDown(SDLK_w), IsKeyDown(SDLK_s));
}

//*************** 2D RENDER ***************
void CMyGame::OnDraw(CGraphics* g)
{

	if (IsMenuMode())
	{
		startScreen.Draw(g);
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
	// start screen
	startScreen.LoadImage("startScreen.jpg");
	startScreen.SetSize((float)Width, (float)Height);
	startScreen.SetPosition((float)Width / 2, (float)Height / 2);
}

//*************** KEYBOARD EVENTS ***************
void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//Exit
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();

	//Start
	if (sym == 13) {

		StartGame();
		mainBgMusic.Play("mainBg.wav");
	}
}

void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{
	player->OnRButtonDown(ScreenToFloorCoordinate(x, y));
}
