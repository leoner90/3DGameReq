#pragma once

//Forwarding Classes
class CCamera;
class Map;
class Enemy;
class Portal;
class UIDialogBox;
class Shop;

class Player 
{
public:
	Player();

	//Health
	float playerMaxHp, playerCurrentHp, maxEnergy, CurrentEnergy, playerCurrentArmor,playerMaxArmor;

	//playerShots 
	CModelList playerShots;
	CModel bullet;

	float playerDamage;
	float shootingDelay;
	float attackDelay;
 

	//local variables
	Map* localMap;
	Portal* localPortal;
	
	//Main Functions
	void init();
	void OnUpdate(Uint32 t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies, CVector& mousePos, Portal& portal);
	void OnDraw(CGraphics* g, UIDialogBox& dialogBox, Shop& shop);
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnRender3D(CGraphics* g, CCamera& world);
	void OnLButtonDown(CVector pos, CVector currentMousePos, long t);
	void playerOnWheelUp();
	void playerOnWheelDown();
	void OnKeyDown(SDLKey sym , CVector currentMousePos);
	void playerGettingDamage(float damage);
	void playerCollision(std::vector<Enemy*> AllEnemies);
	void addLoot(int enemyType, CVector enemyPos);
	
	//player movement
	CVector lastFramePos;


	//3D Models
	CModelMd3 playerModel;

 

	//Player Current Skill
	enum PlayerSkills{DASH, RECALL};
	int curentSkillSelected;

	//Sounds
	CSoundPlayer footsteps;
	CSoundPlayer shotSound;
	bool isPlayerMoving;
	
	enum playerStates { UNOCCUPIED, INATTACK, INDASH };
	playerStates playerCurrentState;

	CFont font;

	//resouces
	int  armorComponents, weaponComponents, bossLoot;

	//death handler
	bool isPlayerDead;
	bool playerPreDeahAnimation;
	Uint32 playerdeathAnimationTimer;

	float prevFrameTime , deltatime;
	void OnMouseMove(CVector currentMousePos);
 
	CVector* localMouse;

	bool onStartGameEvent, firstBlinkyMeet;

	CModelList dashEffect;
	CModel dashVfx, onDamageVfx;

	CModelList lootList, onHitEffect;
	float currentExp;

	//loot
	CModel lootItemOne, lootItemTwo, lootItemThree;

	float dashCoolDown;

private:
	long localTime;
};