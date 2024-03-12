#pragma once
 
class Map
{
public:
	//Main Functions
	void init();
	void OnUpdate(int t);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);

	void LoadData();
	CFloor floor;
	CModel portal, portalPartOne, portalPartTwo, portalPartThree, portalPartFour;
	CModel wallSegment;
	CModel tree,collidingTree, rock,grass, flowers, mushroom;
	CModelList modelList, mapCollision, collidingObjects;
	CFont font;

	Uint32 TotaltimeForPortalToCharg;
	Uint32 currentportalChargingTime;

	
};