#pragma once

class Player;

class Enemy
{
public:
	void init(int posX, int poxY, int posZ, int enemyType);
	void OnUpdate(long t, Player& player);
	void OnDraw(CGraphics* g);

	void OnRender3D(CGraphics* g);
	//void EnemyControl();
	CModelMd2 enemyModel;
	int localEnemyType;

	Player* localPlayer;
};