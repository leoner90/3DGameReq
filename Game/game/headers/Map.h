#pragma once
 
class Map
{
public:
	//Main Functions
	void init();
	void OnUpdate(int t);
	void OnDraw(CGraphics* g);
	void OnRender3D(CGraphics* g);
	CFloor floor;
	CModel portal;
};