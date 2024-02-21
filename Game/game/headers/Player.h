#pragma once

//Forwarding Classes
class CCamera;
class Map;
class Enemy;

class Player 
{
public:
	enum playerStates { UNOCCUPIED, INATTACK};
	int playerCurrentState;
	float attackDelay;
	//Main Functions
	void init();
	void OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey, Map& map, std::vector<Enemy*> AllEnemies);
	void OnDraw(CGraphics* g);
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnRender3D(CGraphics* g, CCamera& world);
	void OnRButtonDown(CVector pos, CVector currentMousePos, long t);
	void OnKeyDown(SDLKey sym , CVector currentMousePos);

	void playerCollision(std::vector<Enemy*> AllEnemies);
	CVector SaveMousePos;

	bool mouseControl;
	//playerShots 
	CModelList playerShots;
	//3D Models
	CModel playerModel;

	CModel bullet;
	//Sounds
	CSoundPlayer footsteps;
	CSoundPlayer shotSound;
};