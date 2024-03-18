#pragma once

class Player;
class Map;
class Portal;

class Enemy
{
public:
	~Enemy();
	void init(int posX, int poxY, int posZ, int enemyType, Map& map, Portal& portal);
	void OnUpdate(Uint32 t, Player& player, Map& map, std::vector<Enemy*>& AllEnemies, int thisEnemyIndex, Portal& portal);
	void OnDraw(CGraphics* g, CVector enemyPos);
	void Attack();
	void OnRender3D(CGraphics* g);
	void EnemyGetDamage(float damage);
	//void EnemyControl();
 
	CModelMd3* enemyModel;

	//local
	Player* localPlayer;
	Map* localMap;
	Portal* localPortal;
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
 
};