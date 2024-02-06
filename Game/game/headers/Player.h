#pragma once

//Forwarding Classes
class CCamera;

class Player 
{
public:
	//Main Functions
	void init();
	void OnUpdate(bool GameOption , long t, bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnDraw(CGraphics* g);
	void PlayerControl(bool Dkey, bool Akey, bool Wkey, bool Skey);
	void OnRender3D(CGraphics* g, CCamera& world);
 
	CVector SaveMousePos;

	//3D Models
	CModelMd2 playerModel;

	//2d player 
	CSprite player2D;

	//2d or 3d game
	bool gameOption;
};