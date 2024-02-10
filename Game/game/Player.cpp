#include "Game.h"
#include "headers/Player.h"
//*************** INIT ***************

void Player::init()
{
	//Player 3D
	playerModel.LoadModel("Player/Abarlith.md2");
	playerModel.SetScale(3.5f);
	playerModel.SetPosition(400, 100, 300);
}

//*************** UPDATE ***************
void Player::OnUpdate( long t, bool Dkey, bool Akey, bool Wkey, bool Skey)
{
	playerModel.Update(t);

	PlayerControl(Dkey, Akey, Wkey, Skey);
}

//*************** 2D RENDER ***************
void Player::OnDraw(CGraphics* g)
{

}

//*************** 3D RENDER ***************
void Player::OnRender3D(CGraphics* g, CCamera& world)
{
	playerModel.Draw(g);;	
}

//*************** PLAYER CONTROLER ***************
void Player::PlayerControl(bool Dkey, bool Akey, bool Wkey , bool Skey)
{

	// align direction with rotation
	playerModel.SetDirectionV(playerModel.GetRotationV());

	//Movement
	if (playerModel.IsAutoMoving())
	{
		playerModel.PlayAnimation("run", 7, true);
	}
	else
	{
		footsteps.Stop();
		playerModel.SetSpeed(0);
		playerModel.PlayAnimation("stand", 7, true);
	}
	
}


void Player::OnRButtonDown(CVector pos)
{
	
	 playerModel.SetRotationToPoint(pos.x, pos.z);
	//	player->playerModel.SetDirection(player->playerModel.GetRotation());
	 playerModel.MoveTo(pos.x, pos.z, 400);
	 footsteps.Stop();
	 footsteps.Play("playerFootsteps.wav", -1);
	 footsteps.SetVolume(10);
}