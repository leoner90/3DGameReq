#pragma once

class Player;
class Map;
class Portal;

class Enemy
{
public:
	Enemy();
	~Enemy();

	//main Functions
	void init(CVector enemyPos, int enemyType, Map& map, Portal& portal, CModelMd3& enemyModel);
	void OnUpdate(Uint32 t, Player& player, Map& map, std::vector<Enemy*>& AllEnemies, int thisEnemyIndex, Portal& portal);
	void OnDraw(CGraphics* g, CVector enemyPos);

	void Attack();
	void OnRender3D(CGraphics* g);
	void EnemyGetDamage(float damage, CModel Vfx);
	//void EnemyControl();
 
	CModelMd3* enemyModel;

	//TO DO GETTERS
	bool isDead;
	bool preDeahAnimation;
	bool OnSpawnHold;

private:
	//local
	Player* localPlayer;
	Map* localMap;
	Portal* localPortal;
	Uint32 localTime;
	int localEnemyType;

	//boss Attacks
	float bossChargedAttackDelay, chargingAttack;
	bool isBossChargingAttack;


	CModelList onHitEffect;
	CModel Vfx;
	CSoundPlayer deathSound;

	Uint32 deathAnimationTimer;
	CHealthBar enemyHpbar;
	CVector randomPortalPartPos;

	//attack
	float enemyDamage;
	float attackDelay;

	//enemy stats
	float enemySpeed;
	float enemyMaxHp, enemyCurrentHp;
};