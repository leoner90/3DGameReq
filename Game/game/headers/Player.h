#pragma once

//Forwarding Classes
class CCamera;

class Player 
{
public:
	//Main Functions
	void init();
	void OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnDraw(CGraphics* g);
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnRender3D(CGraphics* g, CCamera& world);
	void OnRButtonDown(CVector pos);
	CVector SaveMousePos;

	//3D Models
	CModelMd2 playerModel;

	//Sounds
	CSoundPlayer footsteps;
};