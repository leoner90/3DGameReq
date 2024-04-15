#pragma once

//Forwarding Classes
class Map;
class Enemy;
class Portal;
class UIDialogBox;
class Shop;

class Player 
{
public:
	Player();

	//Main Functions
	void init();
	void OnUpdate(Uint32 t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies, CVector& mousePos, Portal& portal, CVector playerWorldPos);
	void OnDraw(CGraphics* g, UIDialogBox& dialogBox, Shop& shop);
	void OnRender3D(CGraphics* g);

	//Main Controler
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);

	//Getting Damage Fucntion
	void playerGettingDamage(float damage);

	//Collisions Handler
	void playerCollision(std::vector<Enemy*> AllEnemies);

	//Loots
	void addLoot(int enemyType, CVector enemyPos);
	void lootHandler();

	//shots
	void playerShotsHandler();
	void performShot();

	//Mouse And Keyboard Controll
	void OnRButtonDown(long t);
	void OnRButtonUp();
	void OnMouseMove(CVector currentMousePos);
	void OnLButtonDown(CVector pos, CVector currentMousePos, long t);
	void playerOnWheelUp();
	void playerOnWheelDown();
	void OnKeyDown(SDLKey sym, CVector currentMousePos);
	
	//GETTERS
		//comming soon,probably!
	//SETTERS
		//comming soon,probably!

	CModelMd3 playerModel;

	//Player Current Skill
	enum PlayerSkills{DASH, RECALL};
	int curentSkillSelected;

	//resources
	int armorComponents, weaponComponents, bossLoot;

	//death handler
	bool isPlayerDead;
	bool playerPreDeahAnimation;

	//exp
	float currentExp;

	//Energy/Armor Regen
	float energyRegenPerSec, armorRegenPerSec;

	//Health , Enerrgy, Armor
	float playerMaxHp, playerCurrentHp, maxEnergy, CurrentEnergy, playerCurrentArmor, playerMaxArmor;
	int curentDashAmount, maxDashAmount;

	//shooting
	float playerDamage, chargedDamage;
	float shootingDelay, attackDelay; // rename to shootingRate

	CSoundPlayer footsteps; // todo as is called from mygame cpp
	float dashCoolDown;
private:
	//Fonts
	CFont font;

	//Player States
	enum playerStates { UNOCCUPIED, INATTACK, INDASH, INDAMAGE };
	playerStates playerCurrentState;
	playerStates savedPrevPlayerState;

	//local variables - mirroring
	long localTime;
	Map* localMap;
	Portal* localPortal;
	std::vector<Enemy*> localEnemies;
	CVector* localMouse;

	//player Shots 
	CHealthBar ShotChargeBar; 
	float ShotChargeBarOffset;
	CModelList playerShots;
	CModel bullet;

	//loot
	CModel lootItemOne, lootItemTwo, lootItemThree;

	//Sounds
	CSoundPlayer shotSound, hitSound, deathSound, dashSound, PickUpLootSound, charginShotSound;

	//Death Handler
	Uint32 playerdeathAnimationTimer;

	// stop and start Footstep.wav
	bool isPlayerMoving;

	//Call Dialog boxes
	bool onStartGameEvent, firstBlinkyMeet;

	//DeltaTime
	float prevFrameTime, deltatime;

	//Charget Shot
	bool chargedShot, isShotCharged;
	float startChargedShotTimer;
	float totalTimeToCharge;

	//Dash
	float dashTimer;

	//Hit Stan
	bool isPlayerInDamage;
	float InDamageStunDelayTimer, repeatStunDelayTimer;

	//VFX
	CModelList dashEffect;
	CModel dashVfx, onDamageVfx;
	CModelList onHitEffect;

	//Loot
	CModelList lootList;

	//save Pos
	CVector lastFramePos;

	//Charged Shot Hanlder
	void chargedShotHandler();
};