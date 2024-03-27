#pragma once
class Player;

class Map
{
public:
	Map();
	//Main Functions
	void init();
	void OnUpdate(Uint32 t, Player& player);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);

	CModelList collidingObjects, mapCollision;
private:
	void weather();
	void LoadData();

	Uint32 localTime;
	CFloor floor;
	CModel wallSegment;
	CModel tree, collidingTree, rock, grass, flowers, mushroom, rainDrop;
	CModelList modelList, rain;
	CFont font;
	CModel shipModel;
	Player* localPlayer;
};