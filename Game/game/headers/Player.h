#pragma once

//Forwarding Classes
class CCamera;
class Map;
class Enemy;

class Player 
{
public:
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

	
	//Main Functions
	void init();
	void OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies);
	void OnDraw(CGraphics* g);
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnRender3D(CGraphics* g, CCamera& world);
	void OnLButtonDown(CVector pos, CVector currentMousePos, long t);
	void playerOnWheelUp();
	void playerOnWheelDown();
	void OnKeyDown(SDLKey sym , CVector currentMousePos);
	void playerGettingDamage(float damage);
	void playerCollision(std::vector<Enemy*> AllEnemies);
	
	//player movement
	CVector lastFramePos;

	

	//3D Models
	CModelMd3 playerModel;

	
	CVector SaveMousePos;

	//Player Current Skill
	enum PlayerSkills{RECALL,DASH};
	PlayerSkills curentSkillSelected;

	//Sounds
	CSoundPlayer footsteps;
	CSoundPlayer shotSound;

	
	enum playerStates { UNOCCUPIED, INATTACK };
	playerStates playerCurrentState;

	CFont font;

	//resouces
	int  armorComponents, weaponComponents;

	bool isPlayerDead;
};