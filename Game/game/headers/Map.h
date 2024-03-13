#pragma once
 
class Map
{
public:
	Map();
	//Main Functions
	void init();
	void OnUpdate(Uint32 t);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);

	void LoadData();
	CFloor floor;
	
	CModel wallSegment;
	CModel tree, collidingTree, rock,grass, flowers, mushroom;
	CModelList modelList, mapCollision, collidingObjects;
	CFont font;



	
};