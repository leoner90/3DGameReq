#pragma once

class Player;
class Map;

class Enemy
{
public:
	void init(int posX, int poxY, int posZ, int enemyType);
	void OnUpdate(long t, Player& player, Map& map);
	void OnDraw(CGraphics* g, CVector enemyPos);
	void Attack();
	void OnRender3D(CGraphics* g);
	void EnemyGetDamage(float damage);
	//void EnemyControl();
 
	CModelMd2 enemyModel;

	//local
	Player* localPlayer;
	Map* localMap;
	Uint32 localTime;


	float enemySpeed;

	int localEnemyType;
	float enemyMaxHp, enemyCurrentHp;
	float enemyDamage;
	bool isDead;
	float attackDelay;
	bool hit;

	CHealthBar enemyHpbar;

};