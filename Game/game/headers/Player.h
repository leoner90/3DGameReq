#pragma once

//Forwarding Classes
class CCamera;
class Map;
class Enemy;

class Player 
{
public:
	//Health
	float maxHp, currentHp, maxEnergy, CurrentEnergy;
	CFont font;


	//shootin
	float playerDamage;
	float shootingDelay;
	float attackDelay;

	//local Map
	Map* localMap;
	enum playerStates { UNOCCUPIED, INATTACK};
	playerStates playerCurrentState;
	
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
	CVector SaveMousePos;

	bool mouseControl;
	//playerShots 
	CModelList playerShots;
	//3D Models
	CModelMd3 playerModel;

	CModel bullet;

	//Player Current Skill
	enum PlayerSkills{RECALL,DASH};
	PlayerSkills curentSkillSelected;
	//Sounds
	CSoundPlayer footsteps;
	CSoundPlayer shotSound;

};