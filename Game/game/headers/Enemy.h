#pragma once

class Player;
class Map;

class Enemy
{
public:
	void init(int posX, int poxY, int posZ, int enemyType, Map& map);
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

	//attack
	float enemyDamage;
	float attackDelay;

	//death handler
	bool isDead;
	bool preDeahAnimation;
	Uint32 deathAnimationTimer;
	CHealthBar enemyHpbar;

	CVector randomPortalPartPos;

	bool OnSpawnHold;

	//temp To Delete
	CModel  spawnPoint, spawnPoint2;

};