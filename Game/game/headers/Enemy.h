#pragma once

class Enemy
{
public:
	void init();
	void OnUpdate();
	void OnDraw(CGraphics* g);
	void EnemyControl();
	void OnRender3D(CGraphics* g);
};