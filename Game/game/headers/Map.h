#pragma once
 
class Map
{
public:
	//Main Functions
	void init();
	void OnUpdate();
	void OnDraw(CGraphics* g);
	void EnemyControl();
	void OnRender3D(CGraphics* g);
};