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
	CModel portal;
	CModel wallSegment;
	CModel tree;
	CModelList modelList, mapCollision;

	
};