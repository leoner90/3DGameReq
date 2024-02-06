#include "Game.h"
#include "headers/Player.h"
//*************** INIT ***************

void Player::init()
{
	//2dPlayer
	player2D.LoadImage("Player/playerRunRight.png" , CColor::Black(), 10);
	player2D.PlayAnimation(0, 9, 9, true);
	player2D.SetPosition(300, 300);
	player2D.PlayAnimation(0,9,9,true);
 

	//Player 3D
	playerModel.LoadModel("Player/Abarlith.md2");
	playerModel.SetScale(3.5f);
	playerModel.SetPosition(400, 100, 300);
}

//*************** UPDATE ***************
void Player::OnUpdate(bool GameOption , long t, bool Dkey, bool Akey, bool Wkey, bool Skey)
{
	gameOption = GameOption;
	if(!gameOption) player2D.Update(t);
	else playerModel.Update(t);

	PlayerControl(Dkey, Akey, Wkey, Skey);
}

//*************** 2D RENDER ***************
void Player::OnDraw(CGraphics* g)
{
	if (!gameOption) player2D.Draw(g);

}

//*************** 3D RENDER ***************
void Player::OnRender3D(CGraphics* g, CCamera& world)
{
	if (gameOption) playerModel.Draw(g);;	
}

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey)
{
	//if 2 D
	if (!gameOption) {
		if (Wkey && Akey) player2D.SetVelocity(-150, 75);
		else if (Wkey && Dkey) player2D.SetVelocity(150, 75);
		else if (Skey && Akey) player2D.SetVelocity(-150, -75);
		else if (Skey && Dkey) player2D.SetVelocity(150, -75);
		else if (Wkey)  player2D.SetVelocity(0, 75);
		else if (Skey)  player2D.SetVelocity(0, -75);
		else if (Akey)  player2D.SetVelocity(-150, 0);
		else if (Dkey)  player2D.SetVelocity(150, 0);
		else player2D.SetVelocity(0, 0);
	}
	//if 3 D
	else {
		// rotate player with RIGHT and LEFT arrow keys
		if (Dkey) playerModel.Rotate(0, -5, 0);
		if (Akey) playerModel.Rotate(0, 5, 0);

		// align direction with rotation
		playerModel.SetDirectionV(playerModel.GetRotationV());

		//Movement
		if (Wkey)
		{
			playerModel.SetSpeed(400);
		 
			playerModel.PlayAnimation("run", 7, true);
		}
		else
		{
			playerModel.SetSpeed(0);
			playerModel.PlayAnimation("stand", 7, true);
		}
	}
}