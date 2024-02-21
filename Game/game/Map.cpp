#include "Game.h"
#include "headers/Map.h"

void Map::init()
{
	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	// set size of the game world
	floor.SetSize(9000, 9000);

	portal.LoadModel("portal/portal.obj");
	//portal.SetSize(1200, 1200, 1200);
	portal.SetScale(12.f);
	portal.SetPosition(400, 0, 300);
	portal.SetHealth(100);
}

void Map::OnUpdate(int t)
{
 
}

void Map::OnDraw(CGraphics* g)
{

}


void Map::OnRender3D(CGraphics* g)
{
	if(portal.GetHealth() > 0) portal.Draw(g);
	floor.Draw(g);
}
