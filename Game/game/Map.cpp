#include "Game.h"
#include "headers/Map.h"

void Map::init()
{
	// floor texture
	floor.LoadTexture("Map/floor2.bmp");
	floor.SetTiling(true);

	// set size of the game world
	floor.SetSize(9000, 9000);
}

void Map::OnUpdate()
{
}

void Map::OnDraw(CGraphics* g)
{

}


void Map::OnRender3D(CGraphics* g)
{
	floor.Draw(g);
}
